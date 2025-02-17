#!/bin/bash

# Create the application directory if it doesn't exist
if [ ! -d /opt/isel/tvs/tvsapp/app ]; then
    mkdir -p /opt/isel/tvs/tvsapp/app
    echo "Created /opt/isel/tvs/tvsapp/app directory."
else
    echo "/opt/isel/tvs/tvsapp/app directory already exists."
fi

# Copy the application files if they haven't been copied already
if [ ! -f /opt/isel/tvs/tvsapp/app/tvsapp.js ]; then
    cp -r ../../app/* /opt/isel/tvs/tvsapp/app/
    echo "Copied application files to /opt/isel/tvs/tvsapp/app."
else
    echo "Application files already exist in /opt/isel/tvs/tvsapp/app."
fi

# Copy the service file to systemd if it doesn't exist
if [ ! -f /etc/systemd/system/tvsapp@.service ]; then
    sudo cp ./tvsapp@.service /etc/systemd/system/
    echo "Copied tvsapp@.service to /etc/systemd/system/."
else
    echo "Service file tvsapp@.service already exists in /etc/systemd/system/."
fi

sudo chown -R $USER:$USER /opt/isel/tvs/tvsapp/app
sudo chmod -R 755 /opt/isel/tvs/tvsapp/app

# Reload systemd manager configuration
sudo systemctl daemon-reload
echo "Systemd manager configuration reloaded."

# Start and enable the service instances if they are not already running
for port in 43211 43212 43213 43214; do
    if ! systemctl is-active --quiet tvsapp@$port; then
        sudo systemctl start tvsapp@$port
        echo "Started tvsapp@$port."
    else
        echo "tvsapp@$port is already running."
    fi

    if ! systemctl is-enabled --quiet tvsapp@$port; then
        sudo systemctl enable tvsapp@$port
        echo "Enabled tvsapp@$port."
    else
        echo "tvsapp@$port is already enabled."
    fi
done

# Check if the Nginx configuration file exists
if [ ! -f /etc/nginx/sites-available/tvsapp ]; then
    cat <<EOF | sudo tee /etc/nginx/sites-available/tvsapp
server {
    listen 44444;

    location / {
        proxy_pass http://tvsapp_backend;
        proxy_set_header Host \$host;
        proxy_set_header X-Real-IP \$remote_addr;
        proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto \$scheme;
    }
}

upstream tvsapp_backend {
    server localhost:43211;
    server localhost:43212;
    server localhost:43213;
    server localhost:43214;
}
EOF
    # Create a symbolic link to enable the site
    sudo ln -s /etc/nginx/sites-available/tvsapp /etc/nginx/sites-enabled/
    # Reload Nginx to apply the new configuration
    sudo systemctl reload nginx
    echo "Nginx configuration for tvsapp created and enabled."
else
    echo "Nginx configuration file /etc/nginx/sites-available/tvsapp already exists."
fi

echo "Service setup completed."