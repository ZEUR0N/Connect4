#!/bin/bash

./Server_Connect4.out 0.0.0.0 2200 &

sleep 1.5

./Client_Connect4.out 127.0.0.1 2200 &

./Client_Connect4.out 127.0.0.2 2200 &