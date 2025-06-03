# Scroll Clicking

This mod allows you to scroll click using your mouse's scroll wheel.

> This mod currently only supports **Windows**.

## Use Case

This mod was created for the intention of beating hard spam levels. While it may be considered cheating in some contexts, it's all up to the player to decide how and when to use it. Use responsibly and at your own discretion.

## Configuration

- **Click Interval** `int` - Controls how many frames to wait between simulated clicks. The lower the value, the faster. `default = 8` `min = 1, max = 20`
- **True Scroll** `bool` - Simulate true scrolling instead of an autoclicker. Experimental feature and may or may not be accurate. `default = false`
- **Player 1 & 2** `bool` - Whether this mod works for the chosen player when dual mode is activated. `default = true`
- **Key for Player 1 & 2** - Mapped keybind for Player 1 and 2. Make sure to customize this in the settings if needed. `P1 default = "SPACE"` `P2 default = "UP"`

## Incompatible Mods

- [Click Between Frames](https://geode-sdk.org/mods/syzzi.click_between_frames) `syzzi.click_between_frames` - Incompatible as the Scroll Clicking mechanism doesn't function when this mod is active. The exact cause is unknown but likely due to input conflict.
- [Click After Frames](https://geode-sdk.org/mods/alphalaneous.click_after_frames) `alphalaneous.click_after_frames` - Incompatible as it delays Scroll Clicks.

## Credits

- Idea made by [HexCodesGMD](https://discord.com/users/1222327935315218506)
- Developed by [SILVERSTAR](https://github.com/silver984) -> [Ko-fi](https://ko-fi.com/silverstar_)
- Built using the [Geode SDK](https://geode-sdk.org/) v4.4.0
