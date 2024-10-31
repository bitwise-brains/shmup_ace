.\tools\palette_conv.exe .\palette\splash_brains.gpl .\data\splash_brains.plt
.\tools\palette_conv.exe .\palette\splash_ace.gpl .\data\splash_ace.plt
.\tools\palette_conv.exe .\palette\game.gpl .\data\game.plt
.\tools\palette_conv.exe .\palette\titlescreen.gpl .\data\titlescreen.plt

.\tools\bitmap_conv.exe .\data\splash_brains.plt splash_brains.png -i -o .\data\splash_brains.bm
.\tools\bitmap_conv.exe .\data\splash_ace.plt splash_ace.png -i -o .\data\splash_ace.bm
.\tools\bitmap_conv.exe .\data\titlescreen.plt titlescreen.png -i -o .\data\titlescreen.bm

.\tools\bitmap_conv.exe .\data\game.plt bigmap.png -i -o .\data\bigmap.bm
.\tools\bitmap_conv.exe .\data\game.plt player.png -mc #FF00FF -i -o .\data\player.bm
.\tools\bitmap_conv.exe .\data\game.plt player_engine.png -mc #FF00FF -i -o .\data\player_engine.bm
.\tools\bitmap_conv.exe .\data\game.plt enemy_small.png -mc #FF00FF -i -o .\data\enemy_small.bm
.\tools\bitmap_conv.exe .\data\game.plt enemy_big.png -mc #FF00FF -i -o .\data\enemy_big.bm
.\tools\bitmap_conv.exe .\data\game.plt explosion_small.png -mc #FF00FF -i -o .\data\explosion_small.bm
.\tools\bitmap_conv.exe .\data\game.plt explosion_big.png -mc #FF00FF -i -o .\data\explosion_big.bm
.\tools\bitmap_conv.exe .\data\game.plt powerups_square.png -mc #FF00FF -i -o .\data\powerups_square.bm
.\tools\bitmap_conv.exe .\data\game.plt hud_icons.png -mc #FF00FF -i -o .\data\hud_icons.bm
.\tools\bitmap_conv.exe .\data\game.plt enemy_bullet_bobs.png -mc #FF00FF -i -o .\data\enemy_bullet_bobs.bm
.\tools\bitmap_conv.exe .\data\game.plt text_gameover.png -mc #FF00FF -i -o .\data\text_gameover.bm
.\tools\bitmap_conv.exe .\data\game.plt text_ready.png -mc #FF00FF -i -o .\data\text_ready.bm
.\tools\bitmap_conv.exe .\data\game.plt text_go.png -mc #FF00FF -i -o .\data\text_go.bm
.\tools\bitmap_conv.exe .\data\game.plt text_title.png -mc #FF00FF -i -o .\data\text_title.bm

.\tools\bitmap_conv.exe sprite.plt enemy_bullet_sprite_1.png -i -o .\data\enemy_bullet_sprite_1.bm
.\tools\bitmap_conv.exe sprite.plt enemy_bullet_sprite_2.png -i -o .\data\enemy_bullet_sprite_2.bm
.\tools\bitmap_conv.exe sprite.plt enemy_bullet_sprite_3.png -i -o .\data\enemy_bullet_sprite_3.bm
.\tools\bitmap_conv.exe sprite.plt enemy_bullet_sprite_4.png -i -o .\data\enemy_bullet_sprite_4.bm
.\tools\bitmap_conv.exe sprite.plt player_bullet_sprite_1.png -i -o .\data\player_bullet_sprite_1.bm
.\tools\bitmap_conv.exe sprite.plt player_bullet_sprite_2.png -i -o .\data\player_bullet_sprite_2.bm
.\tools\bitmap_conv.exe sprite.plt player_bullet_sprite_3.png -i -o .\data\player_bullet_sprite_3.bm
.\tools\bitmap_conv.exe sprite.plt player_bullet_sprite_4.png -i -o .\data\player_bullet_sprite_4.bm

.\tools\font_conv.exe hudfont fnt -size 8 -charFile .\hudfont\charfile.txt -out .\data\hudfont.fnt

.\tools\audio_conv.exe .\sfx\lightlaser.wav -d 2 -o .\data\lightlaser.sfx
.\tools\audio_conv.exe .\sfx\heavylaser.wav -d 2 -o .\data\heavylaser.sfx
.\tools\audio_conv.exe .\sfx\fastlaser.wav -d 2 -o .\data\fastlaser.sfx
.\tools\audio_conv.exe .\sfx\explosion.wav -d 2 -o .\data\explosion.sfx
.\tools\audio_conv.exe .\sfx\collect.wav -d 2 -o .\data\collect.sfx

copy .\data\* ..\build\data