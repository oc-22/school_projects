package router

import (
	"log"
	"net/http"

	"github.com/go-chi/chi/v5"
)

// chiRouter implementuje rozhranie Router pomocou knižnice chi
type chiRouter struct {
	dispatcher *chi.Mux
}

// NewChiRouter vytvorí novú inštanciu routera
func NewChiRouter() Router {
	return &chiRouter{dispatcher: chi.NewRouter()}
}

// GET – registrácia GET endpointu
func (r *chiRouter) GET(uri string, f func(w http.ResponseWriter, r *http.Request)) {
	r.dispatcher.Get(uri, WrapHandler(f, chiExtractParams))
}

// POST – registrácia POST endpointu
func (r *chiRouter) POST(uri string, f func(w http.ResponseWriter, r *http.Request)) {
	r.dispatcher.Post(uri, WrapHandler(f, chiExtractParams))
}

// PUT – registrácia PUT endpointu
func (r *chiRouter) PUT(uri string, f func(w http.ResponseWriter, r *http.Request)) {
	r.dispatcher.Put(uri, WrapHandler(f, chiExtractParams))
}

// SERVE – spustenie HTTP servera
func (r *chiRouter) SERVE(port string) error {
	log.Printf("Chi HTTP server running on port %v", port)
	return http.ListenAndServe(":"+port, r.dispatcher)
}

// chiExtractParams – extrahuje URL parametre z cesty
// teraz už len "gameName", pretože používateľov sme odstránili
func chiExtractParams(r *http.Request) map[string]string {
	return map[string]string{
		"gameName": chi.URLParam(r, "gamename"),
	}
}
