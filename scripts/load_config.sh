#!/bin/bash

# ANPR Configuration Loader
# This script can be run from any location

set -e  # Exit on any error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
CONFIG_FILE="$PROJECT_ROOT/config/anpr.env"

if [ ! -f "$CONFIG_FILE" ]; then
    echo "ERROR: Configuration file not found: $CONFIG_FILE" >&2
    echo "Please create it from the template:" >&2
    echo "  cp config/anpr.env.template config/anpr.env" >&2
    echo "Then edit config/anpr.env with your actual values" >&2
    exit 1
fi

echo "Loading configuration from: $CONFIG_FILE"

set -a
source "$CONFIG_FILE"
set +a

echo "Configuration loaded"
echo "   Project Root: $PROJECT_ROOT"