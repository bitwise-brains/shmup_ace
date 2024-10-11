.\tools\bitmap_conv.exe game.plt bgtiles.png -i -o .\data\bgtiles.bm
.\tools\bitmap_conv.exe game.plt hud_lives_icon.png -mc #FF00FF -i -o .\data\hud_lives_icon.bm
.\tools\bitmap_conv.exe game.plt hud_lives_mark.png -mc #FF00FF -i -o .\data\hud_lives_mark.bm
.\tools\bitmap_conv.exe game.plt hud_special_icon.png -mc #FF00FF -i -o .\data\hud_special_icon.bm
.\tools\bitmap_conv.exe game.plt hud_special_mark.png -mc #FF00FF -i -o .\data\hud_special_mark.bm
.\tools\bitmap_conv.exe game.plt enemy.png -mc #FF00FF -i -o .\data\enemy.bm
.\tools\bitmap_conv.exe game.plt explosion.png -mc #FF00FF -i -o .\data\explosion.bm
.\tools\bitmap_conv.exe game.plt bigexplosion.png -mc #FF00FF -i -o .\data\bigexplosion.bm
.\tools\bitmap_conv.exe game.plt playership.png -mc #FF00FF -i -o .\data\playership.bm
.\tools\bitmap_conv.exe game.plt bullet_bob_enemy.png -mc #FF00FF -i -o .\data\bullet_bob_enemy.bm
.\tools\bitmap_conv.exe game.plt text_ready.png -mc #FF00FF -i -o .\data\text_ready.bm
.\tools\bitmap_conv.exe game.plt text_go.png -mc #FF00FF -i -o .\data\text_go.bm
.\tools\bitmap_conv.exe game.plt text_gameover.png -mc #FF00FF -i -o .\data\text_gameover.bm
.\tools\bitmap_conv.exe sprite.plt bullet_sprite_enemy.png -i -o .\data\bullet_sprite_enemy.bm
.\tools\bitmap_conv.exe sprite.plt bullet_sprite_player.png -i -o .\data\bullet_sprite_player.bm
.\tools\font_conv.exe hudfont fnt -size 8 -charFile "charfile.txt" -out .\data\hudfont.fnt
copy .\data\* ..\build\data