
import sys
import time
from numpy.random.mtrand import f
import pygame
import numpy as np
import wave
import os
# caution: path[0] is reserved for script path (or '' in REPL)
sys.path.insert(1, 'build')

from xemu_module import XemuEmulator

config_path = os.path.dirname(os.path.abspath(__file__))
config_path = os.path.join(config_path, "build/xemu.toml")

xemu = XemuEmulator()
xemu.init(config_path)
xemu.load_state()
xemu.toggle_pause()
xemu.toggle_pause()

height, width = (480, 640)

wav_file = wave.open('xemu_audio.wav', 'wb')
wav_file.setnchannels(2)
wav_file.setsampwidth(2)
wav_file.setframerate(48000)
wav_file.setcomptype('NONE', 'Not Compressed')

pygame.init()


screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Xemu Output")
clock = pygame.time.Clock()

pygame.joystick.init()
joysticks = [pygame.joystick.Joystick(x) for x in range(pygame.joystick.get_count())]

print(f"Detected {len(joysticks)} joystick(s).")

if pygame.joystick.get_count() == 0:
    print("Nothing connected...")
    xemu.deinit()
    exit()

joystick = pygame.joystick.Joystick(0)
joystick.init()

running = True
button_index = 0
count = 0
try:
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                xemu.deinit()
                running = False
                wav_file.close()
                pygame.quit()
                break
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    xemu.deinit()
                    wav_file.close()
                    pygame.quit()
                    running = False
                    break

        buttons = np.zeros(25, dtype=np.uint8)
        xemu.update_input_controller(buttons)

        for i in range(joystick.get_numbuttons()):
            b = joystick.get_button(i)
            if b == 1 and (i != 7 or i != 8):
                buttons[i] = 1
            if b == 1 and i == 6:
                buttons[8] = 1
            if b == 1 and i == 7:
                buttons[9] = 1
    
        hat_x, hat_y = joystick.get_hat(0)

        if hat_x == -1:
            buttons[4] = 1
        elif hat_x == 1:
            buttons[5] = 1
        if hat_y == 1:
            buttons[6] = 1
        elif hat_y == -1:
            buttons[7] = 1

        left_x  = joystick.get_axis(0)
        left_y  = joystick.get_axis(1)
        right_x = joystick.get_axis(2)
        right_y = joystick.get_axis(3)
        lt      = joystick.get_axis(4)
        rt      = joystick.get_axis(5)

    
        if left_y < -0.1:
            buttons[15] = 1
        if left_y > 0.1:
            buttons[16] = 1
        if left_x < -0.1:
            buttons[17] = 1
        if left_x > 0.1:
            buttons[18] = 1

        right_sensitivity = 0.5

        if right_y < -right_sensitivity:
            buttons[20] = 1
        if right_y > right_sensitivity:
            buttons[21] = 1
        if right_x < -right_sensitivity:
            buttons[22] = 1
        if right_x > right_sensitivity:
            buttons[23] = 1

        if lt > 0.1:
            buttons[19] = 1

        if rt > 0.1:
            buttons[24] = 1

        # buttons[button_index] = 1
        # xemu.update_input_controller(buttons)

        # if count % 30 == 0 and count != 0:
        #     button_index += 1
        #     if button_index >= 25:
        #         button_index = 0

        # count+= 1

        xemu.run()
        audio_bytes = xemu.get_audio_bytes()
        wav_file.writeframes(audio_bytes)

        image_bytes = xemu.get_frame_data(width, height)
        frame_data = np.frombuffer(image_bytes, dtype=np.uint8)
        frame_reshaped = frame_data.reshape((height, width, 3))
        frame_reshaped = np.rot90(frame_reshaped, k=3)

        ram_view = xemu.get_system_memory()
        ram_np = np.array(ram_view, dtype=np.uint8)
        # print(ram_np[0x58D544])

        # print(len(xemu.get_system_memory()))
        # print(frame_reshaped)


        frame_surface = pygame.surfarray.make_surface(frame_reshaped)
        
        screen.blit(frame_surface, (0, 0))
        pygame.display.update(0, 0, width, height)
        
        clock.tick(60)
except KeyboardInterrupt:
    wav_file.close()
    pygame.quit()
    print(f"Interrompido no frame \n")
finally:
    wav_file.close()
    pygame.quit()
    print(f"Áudio salvo em 'xemu_audio.wav' frames)")