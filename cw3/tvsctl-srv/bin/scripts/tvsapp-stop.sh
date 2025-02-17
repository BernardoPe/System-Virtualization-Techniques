#!/bin/bash

SITES_AVAILABLE="/etc/nginx/sites-available/tvsapp"
SITES_ENABLED="/etc/nginx/sites-enabled/tvsapp"
DB_OPT="db"
DB_FLAG=false

# Parse command-line arguments
for arg in "$@"; do
    if [ "$arg" == "-db" ]; then
        DB_FLAG=true
    fi
done

PORTS=$(grep -oP 'server 127.0.0.1:\K[0-9]+' $SITES_AVAILABLE)

if [ -z "$PORTS" ]; then
    echo "No tvsapp instances are running"
    exit 0
fi

# Iterate over each service instance and remove it
for port in $PORTS; do
    service="tvsapp@$port"
    if ! systemctl is-active --quiet $service; then
        continue
    fi
    systemctl stop $service
done

if [ "$DB_FLAG" = true ]; then
    # Remove the tvsapp database
    systemctl stop elasticsearch
    echo "Stopped elasticsearch"
fi

# Remove the nginx configuration file from sites-enabled
if [ -f "$SITES_ENABLED" ]; then
    rm $SITES_ENABLED
fi

if systemctl is-active --quiet nginx; then
    systemctl reload nginx
fi

# Reload the systemd manager configuration
systemctl daemon-reload

echo "Tvsapp instances have been stopped"
