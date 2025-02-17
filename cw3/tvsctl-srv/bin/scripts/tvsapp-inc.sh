#!/bin/bash

# Default arguments
DELTA=${1:-1}
SITES_AVAILABLE="/etc/nginx/sites-available/tvsapp"

# Find the highest port currently in use from the Nginx configuration file
LAST_PORT=$(grep -oP 'server 127.0.0.1:\K[0-9]+' $SITES_AVAILABLE | tail -n 1)

# If no ports are found, initialize the LAST_PORT to BASE - 1
if [ -z "$LAST_PORT" ]; then
    LAST_PORT=$((BASE - 1))
fi

# Calculate the BASE port for new instances
BASE=$((LAST_PORT + 1))

# Create the new server lines
new_servers=""
for PORT in $(seq $BASE $((BASE + DELTA - 1))); do
    systemctl start tvsapp@$PORT
    if [ $PORT -eq $((BASE + DELTA - 1)) ]; then
        new_servers="$new_servers    server 127.0.0.1:$PORT;"
    else
        new_servers="$new_servers    server 127.0.0.1:$PORT;\n"
    fi
done

# Insert the new server lines before the marker
sed -i "/# END UPSTREAM BLOCK/i\\$new_servers" $SITES_AVAILABLE

# Reload Nginx to apply the new configuration
if systemctl is-active --quiet nginx; then
    systemctl reload nginx
    echo "Reloaded nginx"
fi

echo "Added $DELTA new instances starting from port $BASE."