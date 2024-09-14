# Medievalution

![image](https://github.com/Lenin-Grub/Medievalution/assets/71728612/1fb4eda4-2caf-4551-9202-437eef591480)

## About
Mdeivalution - мой pet проект, который задуман как игра мечты. Управляйте государством на глобальной карте и сражайтесь на полях битвы в реальном времени. Игра будет охватывать период времени с падения Западной римской империи до падения Константинополя в 1453. Сражайтесь в битвах, торгуйте, заключайте союзы и объявляйте войны.

Проект вдохновлен такими играми как: Victoria II, Crusader Kings II, Europa Universalis IV, Knights of Honor, Total War. 
* Жанр - 4x RTStrategy.

## Development
- [x] Поддержка английского и русского языка
- [x] Конфигурационный файл настроек
- [x] Экран загрузки между сценами
- [x] 2D анимации
- [x] Музыкальный автомат
- [x] Загрузка карты 
- [x] Рисование границ с помощью шейдера
- [x] Поиск пути (временно отключено)
- [x] Управление камерой WSAD
- [x] Cкролинг колесиком мышки
- [ ] Примитивный редактор карт (в разработке)
  

## Сторонние библиотеки:

### Install libraries
> [!WARNING]
> Проверьте что у вас есть все сторонние библиотеки для сборки проекта

> [!IMPORTANT]
> Поместите внешние зависимости в папку External

>[!NOTE]
> Библиотеки должны скачаться автомачтически через fetchcontent в cmake

#### Сторонние библиотеки: 
* sfml 
* spdlog
* ImGui

---

#### Install SFML

* sfml 2.6.0
```sh
git clone https://github.com/SFML/SFML.git
```
---

#### Install ImGui
* sfml ImGui
>[!NOTE]
> ImGui скачается автомачтически через fetchcontent в cmake

---

#### Install SpdLog
* spdlog
```sh
git clone https://github.com/gabime/spdlog.git
```
---

## Build with CMake

>[!TIP]
>Использйте **runscript.bat** для сборки

```sh
git clone https://github.com/Lenin-Grub/Medievalution.git
cd Medievalution
mkdir build
cd build
cmake ..
```
