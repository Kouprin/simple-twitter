set -e
docker build -t kouprin/engine -f engine.docker .
docker build -t kouprin/backend -f backend.docker .
