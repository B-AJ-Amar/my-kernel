docker run --rm \
    -v "$PWD":/workspace \
    -w /workspace \
    kernel-builder \
    make build