set -e
cd images
./build_images.sh
cd ..
docker-compose up --build
