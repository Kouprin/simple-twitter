set -e
docker build -t kouprin/messenger .
docker run -it kouprin/messenger

