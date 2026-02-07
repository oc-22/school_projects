-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2025 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Ondrej Cagalinec <xcagalo00@stud.fit.vutbr.cz>
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru

   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti

   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data

   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_INV  : out std_logic;                      -- pozadavek na aktivaci inverzniho zobrazeni (1)
   OUT_WE   : out std_logic;                      -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'

   -- stavove signaly
   READY    : out std_logic;                      -- hodnota 1 znamena, ze byl procesor inicializovan
   DONE     : out std_logic                       -- hodnota 1 znamena, ze procesor ukoncil vykonavani programu (narazil na instrukci halt)


 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is
    signal cnt_reg : std_logic_vector(11 downto 0);    -- counter of brackets
    signal cnt_load : std_logic;    ---
    signal cnt_inc : std_logic;     --- control signals
    signal cnt_dec : std_logic;     --- of CNT reg
    signal cnt_zero : std_logic;    ---

    signal ptr_reg :  std_logic_vector(12 downto 0);    -- data pointer register
    signal ptr_load : std_logic;    ---
    signal ptr_inc : std_logic;     ---control signals of PTR reg
    signal ptr_dec : std_logic;     ---

    signal pc_reg : std_logic_vector(12 downto 0);    ---Program counter register
    signal pc_inc : std_logic;      ---
    signal pc_dec : std_logic;      ---control signals of PC reg
    signal pc_reset : std_logic;    ---

    signal mx1_sel :  std_logic;    ---selector of which data is stored in DATA_RDATA
    signal mx2_sel : STD_LOGIC_VECTOR(1 downto 0);  ---selector of operations making effect on DATA_WDATA

    ---FSM STATES
    type t_state is (
            s_idle,
            s_init,
            s_load_at,
            s_fetch,
            s_decode,
            s_ptr_inc,s_ptr_dec,
            s_ld_inc, s_write_inc,
            s_ld_dec, s_write_dec,
            s_ld_print, s_print,
            s_set,
            s_halt,
            s_set_hex, s_set_hex_write,
            s_ld_wh, s_wh_cond, s_wh_skipt_evalcnt, s_wh_skipt_check,s_wh_skipt_ld,
            s_ld_wh_e,s_wh_e_cond,s_wh_skipb_evalcnt, s_wh_skipb_check,s_wh_skipb_ld,
            s_continue,
            s_do_start, s_do_end, s_do_skipb_ld,s_do_end_cond, s_do_skipb_check, s_do_skipb_evalcnt


    );
    signal n_state, p_state : t_state;
    signal ready_reg : std_logic := '0';



begin

    pc_reg_proc : process(CLK,RESET)
    begin
        if RESET = '1' then     ---if reset, zero out
            pc_reg <= (others => '0');

        elsif rising_edge(CLK) then ---only change on the rising edge of clock signal
            if pc_dec = '1' then
                pc_reg <= pc_reg - 1;  --- used to jump back during while/do while
            elsif pc_inc = '1' then
                pc_reg <= pc_reg + 1;  --- used to fetch another instruciton
            elsif pc_reset = '1' then ---used to reset when setting PTR
                pc_reg<=(others=>'0');
            end if;
        end if;
    end process;

    ptr_reg_proc : process(CLK,RESET)
    begin
        if RESET = '1' then --- if reset, zero out
            ptr_reg<=(others=>'0');
        elsif rising_edge(CLK) then
            if ptr_load = '1' then  ---setting the PTR to the correct place during inicialization
                ptr_reg<=pc_reg-1;
            elsif ptr_inc= '1' then ---read another bucket of data during >
                if  ptr_reg = "1111111111111" then ---its circular, so reset
                    ptr_reg<=(others=>'0');
                else
                    ptr_reg<=ptr_reg+1; --- if end is not reached just point to another bucket
                end if;
            elsif ptr_dec = '1' then    ---during < operation, set the pointer to the previous bucket
                if ptr_reg = "0000000000000" then   ---circular reset
                    ptr_reg<=(others=>'1');
                else
                    ptr_reg<=ptr_reg-1;  ---regular decrementation
                end if;
            end if;
        end if;
    end process;

    cnt_reg_proc : process(CLK,RESET)
    begin
        if RESET = '1' then     ---zero out on reset
            cnt_reg<=(others=>'0');
        elsif rising_edge(CLK) then ---react on rising CLK
            if cnt_load = '1' then      ---set to one upon entering a cycling state
                cnt_reg<= (others => '0');
                cnt_reg(0)<='1';
            elsif cnt_inc = '1' then    ---increment on {
                cnt_reg<=cnt_reg + 1;
            elsif cnt_dec = '1' then ---decrement on }
                cnt_reg<=cnt_reg-1;
            end if;
        end if;
    end process;

    cnt_zero_proc : process(cnt_reg) ---check if the cnt_reg has reached zero
    begin
        if cnt_reg = x"000" then
            cnt_zero <= '1';    --set control signal if 0
        else
            cnt_zero<= '0';
        end if;
    end process;

    mx1_proc : process(pc_reg,ptr_reg,mx1_sel)
    begin
        if mx1_sel = '1' then
            DATA_ADDR<= ptr_reg;    --- read data
        else
            DATA_ADDR<=pc_reg;  ---read instruction
        end if;
    end process;

    mx2_proc : process(IN_DATA, DATA_RDATA,mx2_sel)
        begin
            if  mx2_sel = "00" then -- ','(input)
                DATA_WDATA <= IN_DATA;
            elsif  mx2_sel = "01" then -- '-'(dec)
                DATA_WDATA <= DATA_RDATA -1;
            elsif mx2_sel = "10" then -- '+' (inc)
                DATA_WDATA <= DATA_RDATA+1;
            elsif mx2_sel = "11" then
                case DATA_RDATA is
                    -- 0-9
                    when x"30" => DATA_WDATA <= x"00";
                    when x"31" => DATA_WDATA <= x"10";
                    when x"32" => DATA_WDATA <= x"20";
                    when x"33" => DATA_WDATA <= x"30";
                    when x"34" => DATA_WDATA <= x"40";
                    when x"35" => DATA_WDATA <= x"50";
                    when x"36" => DATA_WDATA <= x"60";
                    when x"37" => DATA_WDATA <= x"70";
                    when x"38" => DATA_WDATA <= x"80";
                    when x"39" => DATA_WDATA <= x"90";

                    -- A-F
                    when x"41" => DATA_WDATA <= x"A0";
                    when x"42" => DATA_WDATA <= x"B0";
                    when x"43" => DATA_WDATA <= x"C0";
                    when x"44" => DATA_WDATA <= x"D0";
                    when x"45" => DATA_WDATA <= x"E0";
                    when x"46" => DATA_WDATA <= x"F0";

                    when others =>
                        DATA_WDATA <= (others => '0');
                end case;
            end if;
        end process;

    ready_proc : process(CLK, RESET)    ---ready process has to be set upon initialization
    begin
        if RESET = '1' then
            ready_reg <= '0';
        elsif rising_edge(CLK) then
            if p_state = s_init then
                ready_reg <= '1';
            end if;
        end if;
    end process;

    READY <= ready_reg;

    fsm_state : process(CLK, RESET)     ---setting the next_state of fsm
    begin
        if RESET = '1' then ---reset state upon enter
            p_state <=s_idle;
        elsif rising_edge(CLK) then ---set present state upon the change on next state
            p_state<=n_state;
        end if;
    end process;

    fsm : process(p_state,EN,OUT_BUSY,cnt_zero,DATA_RDATA,IN_VLD)
    begin
        ---INITIAL DATA SET----
        pc_inc<='0';
        pc_dec<='0';
        ptr_inc<='0';
        ptr_dec<='0';
        ptr_load<='0';
        cnt_load<='0';
        cnt_inc<='0';
        cnt_dec<='0';
        pc_reset<='0';

        DATA_EN<='0';
        DATA_RDWR<='1';
        IN_REQ<='0';
        OUT_WE<='0';
        OUT_INV<='0';
        OUT_DATA<=(others=>'0');
        DONE<='0';

        mx1_sel<='0';
        mx2_sel<="00";

        case p_state is
            when s_idle=>   ---Dont do anything until EN signal arrives
                if EN = '1' then
                    n_state<=s_load_at; ---then search for the data and instructions symbol '@'
                else
                    n_state<=s_idle;
                end if;

            when s_load_at=>
                ---READ DATA AND INCREMENT PC COUNTER
                DATA_EN<='1';
                DATA_RDWR<='1';
                mx1_sel<='0'; --PC
                if DATA_RDATA = X"40" then  ---If the separator was found, initialize
                    pc_inc<='1';
                    n_state<=s_init;
                else
                    pc_inc<='1';    ---if not, eval another instruction
                    n_state<=s_load_at;
                end if;

            when s_init=>
                ptr_load<='1';  ---set ptr to the first data bucket
                pc_reset<='1';  ---reset pc to the instruction start
                n_state<=s_fetch;

            when s_fetch=>
                 n_state<=s_decode;
                 DATA_RDWR<='1';    ---read next instruction
                 DATA_EN<='1';  ---enable data access

            when s_decode =>
                -----------------------------------
                ----GO THROUGH EACH INSTRUCTION----
                -----------------------------------
                case DATA_RDATA is
                    when x"3E"=>
                        n_state<=s_ptr_inc;
                    when x"3C"=>
                        n_state<=s_ptr_dec;
                    when x"2B"=>
                        n_state<=s_ld_inc;
                    when x"2D"=>
                        n_state<=s_ld_dec;
                    when x"2E"=>
                        n_state<= s_ld_print;
                    when x"2C"=>
                        n_state<=s_set;
                    when x"40"=>
                        n_state<=s_halt;
                    when x"30" | x"31" | x"32" | x"33" | x"34" |
                            x"35" | x"36" | x"37" | x"38" | x"39" |
                            x"41" | x"42" | x"43" | x"44" | x"45" | x"46" =>
                        n_state <= s_set_hex;

                    when x"5B"=>
                        n_state<=s_ld_wh;
                    when x"5D"=>
                        n_state<=s_ld_wh_e;

                    when x"28"=>
                        cnt_load<='1';
                        n_state<=s_do_start;
                    when x"29"=>
                        n_state<=s_do_end;

                    when others =>
                        pc_inc<='1';
                        n_state<=s_fetch;
                end case;

            -- '>'
            when s_ptr_inc=>
                pc_inc<= '1'; --get another instruction
                ptr_inc<='1';
                n_state<=s_fetch;

            -- '<'
            when s_ptr_dec=>
                pc_inc<= '1';   --get another instruction
                ptr_dec<='1';
                n_state<=s_fetch;

            --------------------
            ---DATA AT PTR ++---
            --------------------
            when s_ld_inc=>
                pc_inc <= '1';
                mx1_sel <= '1'; --PTR select
                DATA_RDWR<='1'; --we have to set the ptr in the first clk
                DATA_EN<='1';
                n_state<=s_write_inc;
            when s_write_inc=>
                DATA_RDWR<='0'; --we are writing now
                mx2_sel<= "10"; --select '+' case
                mx1_sel<='1';   -- PTR select
                DATA_EN<='1';
                n_state<=s_fetch;

            ----------------------
            ----DATA AT PTR -- ---
            ----------------------
            when s_ld_dec=>
                pc_inc <= '1';
                mx1_sel <= '1'; --PTR select
                DATA_RDWR<='1'; --set ptr at first clk
                DATA_EN<='1';
                n_state<=s_write_dec;
            when s_write_dec=>
                DATA_RDWR<='0';
                mx2_sel<= "01"; -- select '-' case
                mx1_sel<='1'; --select data write
                DATA_EN<='1';
                n_state<=s_fetch;

            ----------------------
            ------PRINT DATA------
            ----------------------
            when s_ld_print=>
                pc_inc <= '1';
                DATA_RDWR<='1'; --first read
                mx1_sel <='1'; --reading from ptr
                DATA_EN<='1';
                n_state<=s_print;
            when s_print=>
                if OUT_BUSY = '1' then --dont print if some other process si printing rn
                    n_state<=s_print;
                else
                    OUT_WE<='1';    --occupie the read data
                    OUT_DATA<=DATA_RDATA;
                    n_state<=s_fetch;
                end if;

            ----------------------
            -------SET DATA-------
            ----------------------
            when s_set=>
                IN_REQ<='1';
                if IN_VLD = '1' then    --check if the input can be written
                    pc_inc <= '1';
                    mx1_sel <= '1';     --select PTR
                    DATA_RDWR <= '0';   --select writing
                    DATA_EN <='1';
                    mx2_sel <= "00";    --setting from input
                    n_state <=s_fetch;
                else
                    n_state <=s_set;
                end if;

            ----------------------
            ---------HALT---------
            ----------------------
            when s_halt=>   --if '@' was reached
                DONE<='1';
                n_state<=s_halt;

            ----------------------
            ------SET HEX VAL-----
            ----------------------
            when s_set_hex=>
                DATA_RDWR<='1';
                mx1_sel<= '0';
                DATA_EN<='1';
                n_state<=s_set_hex_write;

            when s_set_hex_write=>
                pc_inc <='1';
                mx1_sel<= '1';
                mx2_sel<="11";  --select case to decide the first 4 bits
                DATA_EN<='1';
                DATA_RDWR<='0';
                n_state<=s_fetch;


            --************************************************************
            -----------------------WHILE START----------------------------
            --************************************************************

            --********FETCH ANOTHER INSTRUCTION************
            --*************HELPER_FUNCTION*****************
            when s_continue=>
                mx1_sel<='0';
                pc_inc<='1';
                n_state<=s_fetch;

            --*************************************************
            --************************************************
            -------------------------------
            ---------WHILE START-----------
            ------------CHECK--------------
            -------------------------------
            when s_ld_wh=>  --read the number on ptr
                mx1_sel<='1';
                DATA_EN<='1';
                DATA_RDWR<='1';
                n_state<=s_wh_cond;

            when s_wh_cond=>    --if its zero, while has ended and we have to skip its body
                if DATA_RDATA=x"00"then
                    cnt_load <= '1';
                    pc_inc<='1';
                    n_state<=s_wh_skipt_ld;
                else
                    n_state<=s_continue;    --not zero, do the iteration
                end if;
            -------------------------------
            -------TOP to BOTTOM SKIP------
            -------------------------------
            when s_wh_skipt_ld=>    --read each instruction
                DATA_EN <='1';
                DATA_RDWR<='1';
                mx1_sel<= '0';
                n_state<=s_wh_skipt_check;

            when s_wh_skipt_check=> --count [] to decide whether the while has ended
                n_state<=s_wh_skipt_evalcnt;
                if DATA_RDATA=x"5B" then
                    cnt_inc <= '1';
                elsif DATA_RDATA=x"5D"then
                    cnt_dec <= '1';
                end if;

            when s_wh_skipt_evalcnt=> --if the cnt=0, we have sucessfully skipped the while
                if cnt_zero = '1' then
                    n_state<=s_fetch;
                else
                    n_state<=s_wh_skipt_ld;
                end if;
                pc_inc<='1';    --fetch another instruction

            -----------------------------
            ----------WHILE END----------
            ------------CHECK------------
            -----------------------------
            when s_ld_wh_e=>    --load the current ptr value after an iteration
                mx1_sel<='1';
                DATA_EN<='1';
                DATA_RDWR<='1';
                n_state<=s_wh_e_cond;

            when s_wh_e_cond=>  --evaluate the current ptr value
                if DATA_RDATA =x"00" then --if its zero, we can continue with the instructions outside the while
                    n_state<=s_continue;
                else
                    cnt_load<='1';
                    pc_dec<='1';
                    n_state<=s_wh_skipb_ld; --if not, we have to jump to the beginning of the while
                end if;

            -------------------------------
            -------BOTTOM to TOP SKIP------
            -------------------------------

            when s_wh_skipb_ld=>   --load the current instruction
                DATA_EN <='1';
                DATA_RDWR<='1';
                mx1_sel<= '0';
                n_state<=s_wh_skipb_check;
            when s_wh_skipb_check=> --again, count the [] until they are 0
                if  DATA_RDATA=x"5B" then
                    cnt_dec <='1';
                elsif DATA_RDATA =x"5D" then
                    cnt_inc<='1';
                end if;
                pc_dec<='1';
                n_state<=s_wh_skipb_evalcnt;

            when s_wh_skipb_evalcnt=>   --check the cnt whether its zero
                if cnt_zero = '1' then
                    n_state<=s_continue;
                else
                    n_state<=s_wh_skipb_ld;
                end if;
            --***********************************************************
            ----------------------END OF WHILE---------------------------
            --***********************************************************

            --------------------------
            -----START DO WHILE-------
            --------------------------
            when s_do_start=>   --upon the first arrival at '(' we have to execute the instructions at least once
                n_state<=s_continue;

            --------------------------
            -------END DO WHILE-------
            --------------------------

            when s_do_end=> -- when ')' is reached, read the current ptr value
                mx1_sel<='1';
                DATA_EN<='1';
                DATA_RDWR<='1';
                n_state<=s_do_end_cond;

            when s_do_end_cond=>    --evaluate the ptr value
                if DATA_RDATA =x"00" then   --if its zero, continue outsie do while
                    n_state<=s_continue;
                else
                    cnt_load<='1';
                    pc_dec<='1';
                    n_state<=s_do_skipb_ld; --if not, iterate again
                end if;

            --------------------------
            -----SKIP FROM BOTTOM-----
            --------------------------
            when s_do_skipb_ld=>    --load the current instruction
                DATA_EN <='1';
                DATA_RDWR<='1';
                mx1_sel<= '0';
                n_state<=s_do_skipb_check;
            when s_do_skipb_check=> --count () until 0
                if  DATA_RDATA=x"28" then
                    cnt_dec <='1';
                elsif DATA_RDATA =x"29" then
                    cnt_inc<='1';
                end if;
                pc_dec<='1';
                n_state<=s_do_skipb_evalcnt;

            when s_do_skipb_evalcnt=>   --when () count is 0, continue outside do while
                if cnt_zero = '1' then
                    n_state<=s_continue;
                else
                    n_state<=s_do_skipb_ld;
                end if;

        end case;


    end process;


 -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze
 --   - nelze z vice procesu ovladat stejny signal,
 --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
 --      - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a
 --      - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly.

end behavioral;
