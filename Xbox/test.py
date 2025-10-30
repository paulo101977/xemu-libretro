
import sys
import time
import pygame
import numpy as np
# caution: path[0] is reserved for script path (or '' in REPL)
sys.path.insert(1, 'build')

from xemu_module import XemuEmulator
xemu = XemuEmulator()
xemu.init()

height, width = (480, 640)

pygame.init()
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Xemu Output")
clock = pygame.time.Clock()

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False

    xemu.run()
    image_bytes = xemu.get_frame_data(width, height)
    frame_data = np.frombuffer(image_bytes, dtype=np.uint8)
    frame_reshaped = frame_data.reshape((height, width, 3))
    frame_reshaped = np.rot90(frame_reshaped, k=3)
    # print(frame_reshaped)

    frame_surface = pygame.surfarray.make_surface(frame_reshaped)
    
    screen.blit(frame_surface, (0, 0))
    pygame.display.update(0, 0, width, height)
    
    # clock.tick(60)