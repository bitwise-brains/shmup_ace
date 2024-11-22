.\tools\palette_conv.exe .\palette\splash_brains.gpl .\data\splash_brains.plt
.\tools\palette_conv.exe .\palette\splash_ace.gpl .\data\splash_ace.plt
.\tools\palette_conv.exe .\palette\game.gpl .\data\game.plt
.\tools\palette_conv.exe .\palette\highscore.gpl .\data\highscore.plt

.\tools\bitmap_conv.exe .\data\splash_brains.plt splash_brains.png -i -o .\data\splash_brains.bm
.\tools\bitmap_conv.exe .\data\splash_ace.plt splash_ace.png -i -o .\data\splash_ace.bm

.\tools\bitmap_conv.exe .\data\game.plt tiles_stage1.png -i -o .\data\tiles_stage1.bm
.\tools\bitmap_conv.exe .\data\game.plt tiles_stage2.png -i -o .\data\tiles_stage2.bm
.\tools\bitmap_conv.exe .\data\game.plt tiles_stage3.png -i -o .\data\tiles_stage3.bm
.\tools\bitmap_conv.exe .\data\game.plt tiles_boss.png -i -o .\data\tiles_boss.bm
.\tools\bitmap_conv.exe .\data\game.plt player.png -mc #FF00FF -i -o .\data\player.bm
.\tools\bitmap_conv.exe .\data\game.plt player_engine.png -mc #FF00FF -i -o .\data\player_engine.bm
.\tools\bitmap_conv.exe .\data\game.plt enemy_small.png -mc #FF00FF -i -o .\data\enemy_small.bm
.\tools\bitmap_conv.exe .\data\game.plt enemy_big.png -mc #FF00FF -i -o .\data\enemy_big.bm
.\tools\bitmap_conv.exe .\data\game.plt boss_damage.png -mc #FF00FF -i -o .\data\boss_damage.bm
.\tools\bitmap_conv.exe .\data\game.plt boss_hitflash.png -mc #FF00FF -i -o .\data\boss_hitflash.bm
.\tools\bitmap_conv.exe .\data\game.plt boss_shot.png -mc #FF00FF -i -o .\data\boss_shot.bm
.\tools\bitmap_conv.exe .\data\game.plt explosion_small.png -mc #FF00FF -i -o .\data\explosion_small.bm
.\tools\bitmap_conv.exe .\data\game.plt explosion_big.png -mc #FF00FF -i -o .\data\explosion_big.bm
.\tools\bitmap_conv.exe .\data\game.plt powerups_square.png -mc #FF00FF -i -o .\data\powerups_square.bm
.\tools\bitmap_conv.exe .\data\game.plt hud_icons.png -mc #FF00FF -i -o .\data\hud_icons.bm
.\tools\bitmap_conv.exe .\data\game.plt enemy_bullet_bobs.png -mc #FF00FF -i -o .\data\enemy_bullet_bobs.bm
.\tools\bitmap_conv.exe .\data\game.plt text_gameover.png -mc #FF00FF -i -o .\data\text_gameover.bm
.\tools\bitmap_conv.exe .\data\game.plt text_ready.png -mc #FF00FF -i -o .\data\text_ready.bm
.\tools\bitmap_conv.exe .\data\game.plt text_go.png -mc #FF00FF -i -o .\data\text_go.bm
.\tools\bitmap_conv.exe .\data\game.plt text_title.png -i -o .\data\text_title.bm
.\tools\bitmap_conv.exe .\data\game.plt stage_complete_bg.png -mc #FF00FF -i -o .\data\stage_complete_bg.bm

.\tools\bitmap_conv.exe sprite.plt enemy_bullet_sprite_1.png -i -o .\data\enemy_bullet_sprite_1.bm
.\tools\bitmap_conv.exe sprite.plt enemy_bullet_sprite_2.png -i -o .\data\enemy_bullet_sprite_2.bm
.\tools\bitmap_conv.exe sprite.plt enemy_bullet_sprite_3.png -i -o .\data\enemy_bullet_sprite_3.bm
.\tools\bitmap_conv.exe sprite.plt enemy_bullet_sprite_4.png -i -o .\data\enemy_bullet_sprite_4.bm
.\tools\bitmap_conv.exe sprite.plt player_bullet_sprite_1.png -i -o .\data\player_bullet_sprite_1.bm
.\tools\bitmap_conv.exe sprite.plt player_bullet_sprite_2.png -i -o .\data\player_bullet_sprite_2.bm
.\tools\bitmap_conv.exe sprite.plt player_bullet_sprite_3.png -i -o .\data\player_bullet_sprite_3.bm
.\tools\bitmap_conv.exe sprite.plt player_bullet_sprite_4.png -i -o .\data\player_bullet_sprite_4.bm

.\tools\font_conv.exe hudfont fnt -size 8 -charFile .\hudfont\charfile.txt -out .\data\hudfont.fnt

.\tools\audio_conv.exe .\sfx\shot0.wav -d 2 -o .\data\shot0.sfx
.\tools\audio_conv.exe .\sfx\shot1.wav -d 2 -o .\data\shot1.sfx
.\tools\audio_conv.exe .\sfx\shot2.wav -d 2 -o .\data\shot2.sfx
.\tools\audio_conv.exe .\sfx\shot3.wav -d 2 -o .\data\shot3.sfx
.\tools\audio_conv.exe .\sfx\boss_shot.wav -d 2 -o .\data\boss_shot.sfx
.\tools\audio_conv.exe .\sfx\explosion.wav -d 2 -o .\data\explosion.sfx

.\tools\pak_tool.exe .\data ..\build\data.tte