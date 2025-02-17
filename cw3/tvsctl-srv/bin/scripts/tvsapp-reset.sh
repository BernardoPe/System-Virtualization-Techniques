#!/bin/bash

# Default arguments
SCALE=${1:-1}
BASE=${2:-35000}
SITES_AVAILABLE="/etc/nginx/sites-available/tvsapp"
STOP_SCRIPT="/opt/isel/tvs/tvsctld/bin/scripts/tvsapp-stop.sh"

sh $STOP_SCRIPT

# Start writing to the nginx configuration file
cat <<EOF | tee $SITES_AVAILABLE > /dev/null
upstream tvsapp {
EOF

echo "Creating configuration file..."

# Create the new upstream block
for port in $(seq $BASE $((BASE + SCALE - 1))); do
    # Continue writing to the nginx configuration file
    cat <<EOF | tee -a $SITES_AVAILABLE > /dev/null
    server 127.0.0.1:$port;
EOF
done

# Finish writing the upstream block and add a marker
cat <<EOF | tee -a $SITES_AVAILABLE > /dev/null
# END UPSTREAM BLOCK
}
EOF

# Ensure the server block exists in the nginx configuration file
if ! grep -q "server {" $SITES_AVAILABLE; then
    cat <<EOF | tee -a $SITES_AVAILABLE > /dev/null
server {
    listen 44444 default_server;
    server_name _;

    location / {
        proxy_pass http://tvsapp/;
        proxy_set_header Host \$host;
    }
}
EOF
fi

# Reload nginx if it is running
if systemctl is-active --quiet nginx; then
    systemctl reload nginx
    echo "Reloaded nginx"
fi

echo "Tvsapp instances have been reset"