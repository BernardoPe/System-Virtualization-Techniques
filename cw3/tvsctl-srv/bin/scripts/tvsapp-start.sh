#!/bin/bash

SITES_AVAILABLE="/etc/nginx/sites-available/tvsapp"
# Parse the ports from the nginx configuration file
PORTS=$(grep -oP 'server 127.0.0.1:\K[0-9]+' $SITES_AVAILABLE)

# Iterate over each port and start the corresponding service
for PORT in $PORTS; do
    SERVICE="tvsapp@$PORT"
    systemctl start $SERVICE
    echo "Started $SERVICE"
done

# Create a symbolic link to the nginx configuration file
ln -s /etc/nginx/sites-available/tvsapp /etc/nginx/sites-enabled/
# Enable nginx if it is not already running
if ! systemctl is-active --quiet nginx; then
    echo "Starting nginx"
    systemctl start nginx
    echo "Started nginx"
else
    systemctl reload nginx
fi

if ! systemctl is-active --quiet elasticsearch; then
    echo "Starting elasticsearch"
    systemctl start elasticsearch
    echo "Started elasticsearch"
fi

echo "Tvsapp instances have been started and enabled"