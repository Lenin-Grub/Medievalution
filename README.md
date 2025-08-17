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

## Зависимости

Windows & Linux
C++ 20

> [!IMPORTANT]
> Установите переменную среды External.
> В качестве пути установите любой путь, где хранятся ваши бибилиотеки, либо куда вы хотите чтобы они скачались автоматически.

К примеру для Windows

    External
    D:\External

Либо пропишите нужный вам путь вручную в главном CmakeLists.txt

    set(EXTERNAL_DIR "D:/External")

- нужные зависимости должны скачаться автоматически с GitHub

> [!WARNING]
> Проверьте что у вас есть все сторонние библиотеки для сборки проекта

### Сторонние библиотеки:
* SFML 2.6.x
* SpdLog
* ImGui 1.89
* ImGui-SFML 2.6.x
* EnTT

### Cборка CMake

```sh
git clone https://github.com/Lenin-Grub/Medievalution.git
cd Medievalution
mkdir build
cd build
cmake ..
```

---
### English
- [About the Project](#about-the-project)
- [Dependencies](#dependencies)
- [Installing Libraries](#installing-libraries)
- [CMake Build](#cmake-build)
---

## About the Project

Medievalution is my pet project, envisioned as the game of my dreams. Manage a state on a global map and fight in real-time battles. The game will cover the period from the fall of the Western Roman Empire to the fall of Constantinople in 1453. Fight in battles, trade, form alliances, and declare wars.

The project is inspired by games such as: Victoria II, Crusader Kings II, Europa Universalis IV, Knights of Honor, Total War.
* Genre - 4x RTStrategy.

---

## Dependencies

Windows & Linux
C++ 20

> [!IMPORTANT]
> Set the External environment variable.
> Use any path where your libraries are stored, or where you want them to be automatically downloaded.

For example, for Windows:

    External
    D:\External

Or specify the path manually in the main CMakeLists.txt:

    set(EXTERNAL_DIR "D:/External")

- The necessary dependencies should be automatically downloaded from GitHub

> [!WARNING]
> Make sure you have all the necessary external libraries for building the project

### External Libraries:
* SFML 2.6.x
* SpdLog
* ImGui 1.89
* ImGui-SFML 2.6.x
* EnTT

### CMake Build

```sh
git clone https://github.com/Lenin-Grub/Medievalution.git
cd Medievalution
mkdir build
cd build
cmake ..
```