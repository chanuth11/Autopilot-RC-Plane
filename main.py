import time
import pygame

pygame.init()

# images and caption
screen = pygame.display.set_mode((700, 500))

pygame.display.set_caption("Plane Visualizer")
logo = pygame.image.load('globe.png')
pygame.display.set_icon(logo)

snow = pygame.image.load('background.jpeg')
snow = pygame.transform.scale(snow, (500, 500))

plane_art = pygame.image.load('plane.png')
plane_art = pygame.transform.scale(plane_art, (30, 30))

coordinates = "D:\LOG.TXT"
scale_factor = 10000


# functions
def int_finder(alpha, pos_x, pos_y):
    font = pygame.font.Font('freesansbold.ttf', 20)
    display = font.render(str(alpha[pos_x][pos_y]), True, (31, 28, 28))
    return screen.blit(display, ((pos_x * 30) + 35, (pos_y * 30) + 10))


def background():
    screen.fill((5, 14, 57))


def receive_input(string):
    x_pos = ""
    y_pos = ""
    for x in range(len(string)):
        if string[x] == ',' or string[x] == ' ':
            for y in range(x + 1, len(string)):
                y_pos += (string[y])
            break
        x_pos += string[x]
    return float(x_pos), float(y_pos)


def display_stat(stat, pos_x, pos_y):
    font = pygame.font.Font('freesansbold.ttf', 15)
    display = font.render(str(stat), True, 'white')
    return screen.blit(display, (pos_x, pos_y))


def format_time(to_format):
    return str(int(to_format // 60)) + "." + str(round(to_format % 60, 2))


# array (this does not have a function right now)
graph_length = 15
beta = []
alpha = []
for t in range(graph_length):
    beta.append('.')
for x in range(len(beta)):
    alpha.append(beta)

# plane position
original_coor = open(coordinates)
coor_str = original_coor.readlines()
omega = receive_input(coor_str[0])
print(omega)
original_x = omega[0]
original_y = omega[1]
print(original_x)

pos_x = 0.0
pos_y = 0.0

# Stats (these are to be taken with meters and seconds)
velocity = 0.0
altitude = 0.0
time_from_destination = 0.0
time_elapsed = 0.0
distance_traveled = 0.0
distance_remaining = 0.0

t0 = time.time()  # starts time
# game_loop
runner = True
while runner:
    background()
    screen.blit(snow, (0, 0))
    pygame.draw.rect(screen, (14, 38, 125), (505, 10, 190, 360))
    pygame.draw.line(screen, 'black', (35, 445), (460, 445), 2)
    pygame.draw.line(screen, 'black', (247, 20), (247, 445), 2)
    screen.blit(plane_art, (233 + int(pos_x), 430 + int(pos_y)))  # add pos_x to 233 and pos_y to 430 for changes
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            runner = False

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_r:
                t0 = 0.0
                coor_file = open(coordinates)
                current = coor_file.readlines()
                temp = receive_input(current[0])
                original_x = temp[0]
                original_y = temp[1]

    # change in Stats
    t1 = time.time()
    time_elapsed = t1 - t0
    time_elapsed = format_time(round(time_elapsed, 1))
    display_stat("Time Elapsed:", 510, 30)
    display_stat(time_elapsed, 650, 30)

    display_stat("Average Velocity:", 510, 70)
    display_stat(velocity, 650, 70)

    display_stat("Distance Traveled:", 510, 110)
    display_stat(distance_traveled, 650, 110)

    # change in position
    coor_file = open(coordinates)
    current = coor_file.readlines()
    temp = receive_input(current[-1])  # last values of long and lat in the file
    pos_x = (temp[0] - original_x) * scale_factor  # figure out what to multiply these two statements by for proper scaling.
    pos_y = (temp[1] - original_y) * scale_factor
    '''for x in range (len(alpha)):
    if x == 7: continue
    for y in range (len(alpha[0])):
      if y == 14: continue
      int_finder(alpha, x, y)'''
    pygame.display.update()
