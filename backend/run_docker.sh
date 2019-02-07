set -e
docker build -t kouprin/app .
docker run -itd -p 5000:5000 kouprin/app
