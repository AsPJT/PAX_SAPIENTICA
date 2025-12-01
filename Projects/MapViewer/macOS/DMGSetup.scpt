on run argv
  set volumeName to item 1 of argv
  tell application "Finder"
    tell disk volumeName
      open
      set current view of container window to icon view
      set toolbar visible of container window to false
      set statusbar visible of container window to false
      set the bounds of container window to {400, 100, 900, 500}
      set viewOptions to the icon view options of container window
      set arrangement of viewOptions to not arranged
      set icon size of viewOptions to 72
      try
        set background picture of viewOptions to file ".background:DMGBackground.png"
      end try
      delay 1
      set position of item "PAX SAPIENTICA.app" of container window to {125, 200}
      try
        set position of item "Applications" of container window to {375, 200}
      end try
      close
      open
      update without registering applications
      delay 2
    end tell
  end tell
end run
