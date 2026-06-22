#!/usr/bin/env bash

if [ -f ".test_success" ]; then
    rm .test_success
fi

while [[ $# -gt 0 ]]; do
    case $1 in
        --clean)
            CLEAN=true
            shift
            ;;
        --server)
            SERVER=true
            shift
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

pushd frostbyte || exit 1

if [[ $CLEAN ]]; then
    if [[ $SERVER ]]; then
        rm -rf build-headless || exit 1
    else
        rm -rf build || exit 1
    fi
fi

if [[ $SERVER ]]; then
    ./mate --headless || exit 1
    popd || exit 1
    pushd frostbyte-server || exit 1
    rm -rf build && ./mate ../frostbyte || exit 1
    popd || exit 1
else
    ./mate ../frostbyte-desktop/dependencies/rlImGui || exit 1
    popd || exit 1
    pushd frostbyte-desktop || exit 1
    rm -rf build && ./mate ../frostbyte || exit 1
    popd || exit 1
fi

echo "done"
