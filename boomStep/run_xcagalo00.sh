#!/bin/bash

echo "=========================================="
echo "   Running BOOMstep (Backend + Frontend)"
echo "=========================================="

cleanup() {
    if [ -n "$BACKEND_PID" ]; then kill $BACKEND_PID; fi
    if [ -n "$FRONTEND_PID" ]; then kill $FRONTEND_PID; fi
    exit
}

trap cleanup SIGINT

echo "Running backend..."
cd backend

go mod download

go run cmd/minesweeper-api/main.go &
BACKEND_PID=$!
cd ..

echo "Waiting for backend to start..."
sleep 2

echo "Running frontend..."
cd frontend_xcagalo00

if [ ! -d "node_modules" ]; then
    echo "Downloading dependencies..."
    npm install
fi

echo "Starting development server..."
npm run dev &
FRONTEND_PID=$!
cd ..

echo ""
echo "ALL IS RUNNING"
echo "Backend PID: $BACKEND_PID"
echo "Frontend PID: $FRONTEND_PID"
echo "Use ctrl+c to stop"

wait
