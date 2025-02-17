#!/bin/bash

SITES_AVAILABLE="/etc/nginx/sites-available/tvsapp"
PORTS=$(grep -oP 'server 127.0.0.1:\K[0-9]+' $SITES_AVAILABLE)

# Function to check the status of a service
check_service_status() {
    local SERVICE_NAME=$1
    if systemctl is-active --quiet $SERVICE_NAME; then
        echo "$SERVICE_NAME: active"
    else
        echo "$SERVICE_NAME: inactive"
    fi
}

# Check the status of Nginx
check_service_status nginx

# Check the status of the web applications for each port
for PORT in $PORTS; do
    check_service_status tvsapp@$PORT
done

# Check the status of the database (assuming the service name is 'db')
check_service_status elasticsearch