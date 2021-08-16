import pygame,sys
import time
import math
from random import randrange
#config
width, height = 1920, 1080
x0 = width / 2
y0 = height - 10

pygame.font.init()

#game initialization
pygame.init()
myfont = pygame.font.SysFont('Comic Sans MS', 18)
window = pygame.display.set_mode((width, height), 0, 32)
pygame.display.set_caption('Basic Pygame Template')
clock = pygame.time.Clock()

#Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
GRAY = (125, 125, 125)
LIGHT_BLUE = (64, 128, 255)
GREEN = (0, 200, 64)
YELLOW = (225, 225, 0)
PINK = (230, 50, 230)
RED  = (255,0,0)

#Circle
circles = []
step = 1
angle = 0
r = width / 2 - 5

while (angle <= 180):
    x = x0 + r * math.cos(math.radians(angle))
    y = y0 - r * math.sin(math.radians(angle))
    circles.append([x,y])
    angle+=step

#Lines 
lines = []
step = 30
angle = 0

while(angle <= 180):
    x = x0 + r * math.cos(math.radians(angle))
    y = y0 - r * math.sin(math.radians(angle))
    lines.append([x,y])
    angle += step 

#Text
def DrawText(scr, text, x, y, color):
    global myfont 
    textsurface = myfont.render(text, False, color)
    scr.blit(textsurface,(x , y))

def CalcDist(x0,y0,x1,y1):
    return math.sqrt((x1 - x0)**2 + (y1-y0) ** 2)

angles = []
angle = 0

while(angle <= 180):
    angles.append(angle)
    angle+=step

surface1 = pygame.Surface((width,height))
surface1.set_colorkey((0,0,0))
surface1.set_alpha(6)

moving_line_angle = 0
moving_line_step  = 0.1

targets = []

for i in range(50):
    targets.append([randrange(0,width), randrange(0, height)])

DrawText(window, "Step: " + str(moving_line_step), 10,30, YELLOW)
founded_targets = []
#game loop
while True:

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    pygame.draw.rect(surface1, (1,1,1), (0,0,width,height))

    #RADAR
    for circle in circles:
        pygame.draw.circle(window, YELLOW, (circle[0], circle[1]), 1)

    for line in lines:
        pygame.draw.line(window, WHITE, [x0, y0], [line[0], line[1]])

    for angle in range(0,len(lines), 1):
        DrawText(window, str(angles[angle]), lines[angle][0] - 10, lines[angle][1] - 20, (255,0,0))
    
    window.blit(surface1, (0,0))   
    pygame.draw.line(window, GREEN, [x0,y0], [x0 + r * math.cos(math.radians(moving_line_angle)),  y0 - r * math.sin(math.radians(moving_line_angle))])

    for target in targets:
        dist = CalcDist(x0,y0,target[0], target[1])

        if(target[0] == math.floor(x0 + math.cos(math.radians(moving_line_angle)) * dist)):
            if(dist < r and target[1] <= y0):
                founded_targets.append(target)

    for target in founded_targets:
        pygame.draw.circle(window, RED, target, 4)
        DrawText(window, "(" + str(target[0]) + "," + str(target[1]) + ")", target[0] + 5, target[1] - 10, (255,255,255))

    moving_line_angle += moving_line_step

    if(moving_line_angle >= 180 or moving_line_angle <= 0):
        moving_line_step = -moving_line_step
        founded_targets.clear()

    pygame.display.update()
    clock.tick(120)

