# Controller-Plant-Simulation

This cpp project aims to test controller applications with their plant which runs in another process. Also, the project is targeting to present a library for IPC with fixed message structures.

## How to Intall google test

You need to follow the instructions stated below link

https://github.com/google/googletest/blob/master/googletest/README.md

## How To Build

```shell
mkdir build
```
## How To Install

```shell
./automate.sh install
```

## How To Run
```shell
./automate.sh build
./automate.sh run plant
```

```another shell
./automate.sh run controller
```


## Demo Run

In order to show how it looks like. A demo run is executed. Below image shows the screenshot of the plant application. Basicly, it creates a noisy signal and at the background controller is processing the signal and responses to the plant application.
![Plant output screenshot](https://github.com/dogukanarat/controller-plant-simulation/blob/master/docs/Images/plant_ss.png)
