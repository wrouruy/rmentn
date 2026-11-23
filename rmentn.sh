#!/usr/bin/env bash

recursive_rm() {
    local path="${1:-.}"
    local mention=$2

    shopt -s dotglob nullglob

    for e in "$path/"*; do
        if [[ -z "$e" ]]; then
            continue
        elif [[ "${e##*/}" == "$mention" ]]; then
            rm -fri "$e"
        elif [[ -d "$e" ]]; then
            recursive_rm "$e" $2
        fi
    done
}

recursive_rm $1 $2