#!/bin/bash

# Default arguments
DELTA=${1:-1}
SITES_AVAILABLE="/etc/nginx/sites-available/tvsapp"

# Extract the ports from the Nginx configuration file
PORTS=$(grep -oP 'server 127.0.0.1:\K[0-9]+' $SITES_AVAILABLE | sort -nr)

# Calculate the number of instances to remove
NUM_PORTS=$(echo "$PORTS" | wc -l)
if [ $NUM_PORTS -le 1 ]; then
    echo "Cannot remove instances. At least one instance must remain."
    exit 1
fi

# Ensure we do not remove more instances than available
if [ $DELTA -ge $NUM_PORTS ]; then
    DELTA=$((NUM_PORTS - 1))
fi

# Get the ports to remove
PORTS_TO_REMOVE=$(echo "$PORTS" | head -n $DELTA)
END_LINE=$(grep -n "# END UPSTREAM BLOCK" $SITES_AVAILABLE | cut -d: -f1)

# Stop and disable the instances with the highest ports
for PORT in $PORTS_TO_REMOVE; do
    SERVICE="tvsapp@$port"
    systemctl stop $service
    END_LINE=$((END_LINE - 1))
    sed -i "${END_LINE}d" $SITES_AVAILABLE
done

# Reload Nginx to apply the new configuration
if systemctl is-active --quiet nginx; then
    systemctl reload nginx
    echo "Reloaded nginx"
fi

echo "Removed $DELTA instances with the highest ports."