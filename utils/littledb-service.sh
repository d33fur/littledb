#!/bin/bash

SERVICE_NAME=littledb
BIN_PATH="/usr/local/bin/littledb-server"
PID_PATH="/var/run/$SERVICE_NAME.pid"

case $1 in
    start)
        echo "Starting $SERVICE_NAME..."
        if [ -f $PID_PATH ]; then
            echo "$SERVICE_NAME is already running..."
            exit 1
        fi
        $BIN_PATH
    ;;
    stop)
        echo "Stopping $SERVICE_NAME..."
        if [ ! -f $PID_PATH ]; then
            echo "$SERVICE_NAME is not running..."
            exit 1
        fi
        PID=$(cat $PID_PATH)
        kill $PID
        echo "$SERVICE_NAME stopped..."
        rm $PID_PATH
    ;;
    restart)
        echo "Restarting $SERVICE_NAME..."
        $0 stop
        $0 start
    ;;
    status)
        if [ -f $PID_PATH ]; then
            echo "$SERVICE_NAME is running..."
        else
            echo "$SERVICE_NAME is stopped..."
        fi
    ;;
    *)
        echo "Usage: $0 {start|stop|restart|status}"
        exit 1
    ;;
esac
