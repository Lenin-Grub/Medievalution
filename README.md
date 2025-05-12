# Medievalution

![image](https://github.com/Lenin-Grub/Medievalution/assets/71728612/1fb4eda4-2caf-4551-9202-437eef591480)
<div align="center">

[![Image 1](https://github.com/user-attachments/assets/50f5e26f-7dc4-48b5-8d80-c58cc26f97a3)](#русский) [![Image 2](https://github.com/user-attachments/assets/2096b39c-b88c-4657-ae19-7ac93c850331)](#english)

</div>

---
## Оглавление
### Русский
- [О проекте](#о-проекте)
- [Сторонние библиотеки](#сторонние-библиотеки)
- [Установка библиотек](#установка-библиотек)
- [Сборка CMake](#cборка-cmake)
---

## О проекте

Medievalution - мой pet проект, который задуман как игра мечты. Управляйте государством на глобальной карте и сражайтесь на полях битвы в реальном времени. Игра будет охватывать период времени с падения Западной римской империи до падения Константинополя в 1453. Сражайтесь в битвах, торгуйте, заключайте союзы и объявляйте войны.

Проект вдохновлен такими играми как: Victoria II, Crusader Kings II, Europa Universalis IV, Knights of Honor, Total War. 
* Жанр - 4x RTStrategy.

---

## Сторонние библиотеки

Windows
C++ 20

### Установка библиотек
> [!WARNING]
> Проверьте что у вас есть все сторонние библиотеки для сборки проекта

> [!IMPORTANT]
> Поместите внешние зависимости в папку External

>[!NOTE]
> Библиотеки должны скачаться автоматически через fetchcontent в cmake

#### Сторонние библиотеки: 
* SFML 
* SpdLog
* ImGui
* EnTT

---

#### Установка SFML

* sfml 2.6.0
```sh
git clone https://github.com/SFML/SFML.git
```
---

#### Установка ImGui
* sfml ImGui
```sh
git clone [https://github.com/SFML/SFML.git](https://github.com/SFML/imgui-sfml.git)
```
>git tag docking
---

#### Установка EnTT
* entt
```sh
git clone [https://github.com/SFML/SFML.git](https://github.com/skypjack/entt.git)
```
---
#### Установка SpdLog
* spdlog
```sh
git clone git clone https://github.com/gabime/spdlog.git
```
---

## Cборка CMake

>[!TIP]
>Использйте **runscript.bat** для сборки

```sh
git clone https://github.com/Lenin-Grub/Medievalution.git
cd Medievalution
mkdir build
cd build
cmake ..
```
---
---
## Сontents
### English
- [About](#about)
- [Install Libraries](#install-libraries)
- [Third-party Libraries](#external-libraries)
- [Build with CMake](#build-with-cmake)


## About

Medievalution is my pet project envisioned as a dream game. Manage a state on a global map and fight on battlefields in real-time. The game will cover the period from the fall of the Western Roman Empire to the fall of Constantinople in 1453. Fight in battles, trade, form alliances, and declare wars.
The project is inspired by games like: Victoria II, Crusader Kings II, Europa Universalis IV, Knights of Honor, Total War.

Genre - 4x RTStrategy.
---
Windows
C++ 20

### Install libraries
> [!WARNING]
> Check that you have all third-party libraries to build the project

> [!IMPORTANT]
> Place external dependencies in the External folder

>[!NOTE]
> Libraries should be downloaded automatically via fetchcontent in cmake

#### External libraries: 
* SFML 
* SpdLog
* ImGui
* EnTT

---

#### Install SFML

* sfml 2.6.0
```sh
git clone https://github.com/SFML/SFML.git
```
---

#### Install ImGui
* sfml ImGui
```sh
git clone [https://github.com/SFML/SFML.git](https://github.com/SFML/imgui-sfml.git)
```
>git tag docking
---

#### Install EnTT
* entt
```sh
git clone [https://github.com/SFML/SFML.git](https://github.com/skypjack/entt.git)
```
---
#### Install SpdLog
* spdlog
```sh
git clone git clone https://github.com/gabime/spdlog.git
```
---

## Build with CMake

>[!TIP]
>Use **runscript.bat** to fast build

```sh
git clone https://github.com/Lenin-Grub/Medievalution.git
cd Medievalution
mkdir build
cd build
cmake ..
```
