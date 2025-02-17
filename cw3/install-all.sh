#!/bin/bash

APP_IN_DIR="./tvsapp/app"
SERVER_BIN_DIR="./tvsctl-srv/bin"
CLIENT_BIN_DIR="./tvsctl-cli/bin"
APP_SERVICE_IN_FILE="./tvsapp/etc/service/tvsapp@.service"

SERVER_SERVICE_IN_FILE="./tvsctl-srv/etc/service/tvsctld.service"
SERVER_SOCKET_IN_FILE="./tvsctl-srv/etc/socket/tvsctld.socket"

CLIENT_EXECUTABLE="tvsctl"
SERVER_EXECUTABLE="tvsctld"

APP_EXECUTABLE="tvsapp.js"

APP_OUT_DIR="/opt/isel/tvs/tvsapp/app"
CLIENT_OUT_DIR="/opt/isel/tvs/tvsctl/bin"
SERVER_OUT_DIR="/opt/isel/tvs/tvsctld/bin"

SYSTEMD_DIR="/etc/systemd/system"

GROUP="tvsgrp"
USER="isel"

if [ $UID != 0 ]; then
    echo "Please run this script as a superuser."
    exit 1
fi

# Create the directories if they don't exist

if [ ! -d $APP_OUT_DIR ]; then
    mkdir -p $APP_OUT_DIR
    echo "Created $APP_OUT_DIR directory."
else
    echo "$APP_OUT_DIR directory already exists."
fi

if [ ! -d $CLIENT_OUT_DIR ]; then
    mkdir -p $CLIENT_OUT_DIR
    echo "Created $CLIENT_OUT_DIR directory."
else
    echo "$CLIENT_OUT_DIR directory already exists."
fi

if [ ! -d $SERVER_OUT_DIR ]; then
    mkdir -p $SERVER_OUT_DIR
    echo "Created $SERVER_OUT_DIR directory."
else
    echo "$SERVER_OUT_DIR directory already exists."
fi

# Copy the binaries if they haven't been copied already
if [ ! -f $APP_OUT_DIR/$APP_EXECUTABLE ]; then
    cp -r $APP_IN_DIR/* $APP_OUT_DIR/
    echo "Copied application files to $APP_OUT_DIR."
else
    echo "Application files already exist in $APP_OUT_DIR."
fi


if [ ! -f $CLIENT_BIN_DIR/$CLIENT_EXECUTABLE ]; then
    ls $CLIENT_BIN_DIR
    echo "Run make before running this script."
    exit 1
fi

cp -f -r $CLIENT_BIN_DIR/* $CLIENT_OUT_DIR/
echo "Copied client binaries to $CLIENT_OUT_DIR."

if [ ! -f $SERVER_BIN_DIR/$SERVER_EXECUTABLE ]; then
    echo "Run make before running this script."
    exit 1
fi

cp -f -r $SERVER_BIN_DIR/* $SERVER_OUT_DIR/
echo "Copied server binaries to $SERVER_OUT_DIR."

# Copy the service file to systemd
cp -f $SERVER_SERVICE_IN_FILE $SYSTEMD_DIR
cp -f $SERVER_SOCKET_IN_FILE $SYSTEMD_DIR
cp -f $APP_SERVICE_IN_FILE $SYSTEMD_DIR

echo "Copied tvsapp@.service to $SYSTEMD_DIR."

# Set up permissions
# Add user to group
if ! getent group $GROUP > /dev/null; then
    groupadd $GROUP
    echo "Created group $GROUP."
fi

if ! getent passwd $USER > /dev/null; then
    useradd -g $GROUP $USER
    echo "Created user $USER."
fi

# Set permissions for directories and files
chown root:$GROUP $SERVER_OUT_DIR $CLIENT_OUT_DIR $APP_OUT_DIR

chgrp -R $GROUP $CLIENT_OUT_DIR $APP_OUT_DIR

# Reload systemd to apply the new service file
systemctl daemon-reload

cd $APP_OUT_DIR

npm install

echo "Installation and setup completed."