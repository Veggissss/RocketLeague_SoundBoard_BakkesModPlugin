# RocketLeague_SoundBoard_BakkesModPlugin 🎧🔊

RocketLeague_SoundBoard_BakkesModPlugin is a soundboard plugin that plays custom sounds during game events in Rocket League. 🎮

## Requirements 🛠️

- Visual Studio 💻
- C++ Compiler 🔧

## Installation ⚙️

1. **Clone or download the repository**. 🔅
2. **Open the project in Visual Studio**. 🗂
3. **Build the project** to generate the plugin files. 🏗️
4. **Move the compiled plugin** to the BakkesMod plugin folder:
   - Copy the `sounds` folder to your BakkesMod directory: `bakkesmod\bakkesmod\data\sounds`. 🗂
     - `crossbar.wav` 🥅
     - `goal.wav` 🏊
     - `aerial_goal.wav` 🏀
     - `demolition.wav` 💥
     - `epic_save.wav` 🛡️
     - `save.wav` 🫄
     - `mvp.wav` ⭐
     - `car_hit_ball.wav` 🚗💥⚽
     - `bump.wav` 🚗💥🚙
     - `jump.wav` 🆙
     - `boost.wav` 🏎
5. **Launch Rocket League** with BakkesMod enabled. ✨

## Changing Sounds 🎶

You can replace the default sounds with your own. To do so:

- **Keep the same file names** as the originals (e.g., `goal.wav`, `save.wav`).
- All sound files **must be in `.wav` format**.
- Additional sounds can be found in the `another-sounds` folder or on websites like [MyInstants](https://www.myinstants.com/en/index/fr/).

### Notes:

- **Change plugin settings**: Press `F2` -> `Plugins` -> `SoundBoardPlugin`.
- **Adjust volume**: Use the plugin settings volume slider to change to global application volume. The in-game slider doesn't affect plugin sounds.
- **Change individual sound volume**: Edit the sound files in an external audio/video editor before importing.
- **Disable a sound**: In the plugin settings, disable the sound you want.
- **Adjust delay**: Change the minimum delay between sound plays in the same menu.

## Usage 🎶

Once the plugin is installed and the sound files are in place, the sounds will trigger automatically during specific in-game events (e.g., hitting the crossbar, scoring a goal). No further setup is required once the plugin is loaded. 🎉

## Contributing 🤝

Feel free to fork the repository, submit issues, and contribute to the project via pull requests. 🔄

## License 📜

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
