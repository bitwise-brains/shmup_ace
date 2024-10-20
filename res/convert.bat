.\tools\bitmap_conv.exe game.plt bgtiles.png -i -o .\data\bgtiles.bm
.\tools\bitmap_conv.exe game.plt player.png -mc #FF00FF -i -o .\data\player.bm
.\tools\bitmap_conv.exe game.plt enemy.png -mc #FF00FF -i -o .\data\enemy.bm
.\tools\bitmap_conv.exe game.plt explosion.png -mc #FF00FF -i -o .\data\explosion.bm
.\tools\bitmap_conv.exe game.plt hud_power_icon.png -mc #FF00FF -i -o .\data\hud_power_icon.bm
.\tools\bitmap_conv.exe game.plt hud_lives_icon.png -mc #FF00FF -i -o .\data\hud_lives_icon.bm
.\tools\bitmap_conv.exe game.plt hud_special_icon.png -mc #FF00FF -i -o .\data\hud_special_icon.bm
.\tools\bitmap_conv.exe game.plt enemy_bullet_bob_1.png -mc #FF00FF -i -o .\data\enemy_bullet_bob_1.bm
.\tools\bitmap_conv.exe game.plt enemy_bullet_bob_2.png -mc #FF00FF -i -o .\data\enemy_bullet_bob_2.bm
.\tools\bitmap_conv.exe sprite.plt enemy_bullet_sprite_1.png -i -o .\data\enemy_bullet_sprite_1.bm
.\tools\bitmap_conv.exe sprite.plt enemy_bullet_sprite_2.png -i -o .\data\enemy_bullet_sprite_2.bm
.\tools\bitmap_conv.exe sprite.plt player_bullet_sprite_1.png -i -o .\data\player_bullet_sprite_1.bm
.\tools\bitmap_conv.exe sprite.plt player_bullet_sprite_2.png -i -o .\data\player_bullet_sprite_2.bm
.\tools\bitmap_conv.exe sprite.plt player_bullet_sprite_3.png -i -o .\data\player_bullet_sprite_3.bm
.\tools\bitmap_conv.exe sprite.plt player_bullet_sprite_4.png -i -o .\data\player_bullet_sprite_4.bm
.\tools\font_conv.exe hudfont fnt -size 8 -charFile "charfile.txt" -out .\data\hudfont.fnt
.\tools\audio_conv.exe .\sfx\laser.wav -d 2 -o .\data\laser.sfx
.\tools\audio_conv.exe .\sfx\explosion.wav -d 2 -o .\data\explosion.sfx

copy .\data\* ..\build\data