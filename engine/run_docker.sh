set -e
docker build -t kouprin/messenger .
docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -itd kouprin/messenger

