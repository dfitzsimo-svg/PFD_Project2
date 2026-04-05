import pygame
import math
from attitude_marks import draw_attitude_marks  
from pitch_bug import draw_pitch_bug
from triangles import draw_triangles
from tape_boxes import draw_speed_tape_values, draw_altitude_tape_values
from curved_scales import draw_roll_marks, draw_heading_marks


# initialise pygame
pygame.init()
WIDTH, HEIGHT = 480, 320
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("737 Standby PFD Prototype")
clock = pygame.time.Clock()

# colours
BLACK = (0, 0, 0)
BLUE  = (0, 102, 204)
BROWN = (153, 102, 51)
GREY  = (100, 100, 100)
WHITE = (255, 255, 255) 
APP_GREEN = (0, 220, 0)
app_font = pygame.font.SysFont("arialblack", 28)  # heavier font

STD_COLOUR = (0, 220, 0)
std_font = pygame.font.SysFont("arialblack", 28)



# margins / geometry
OUTER_GAP = 3
TAPE_WIDTH = 80                                    # not used yet but you will need it later
HORIZON_LEFT = TAPE_WIDTH + 2 * OUTER_GAP             # leave room for future left tape
HORIZON_WIDTH = WIDTH - 2 * HORIZON_LEFT              # Width of artificial horizon area
BLACK_HEIGHT = 40                   # Height of black bars at top and bottom of screen                           

HORIZON_HEIGHT = HEIGHT - (BLACK_HEIGHT * 2)          # Height of combined orange and blue sections
BLUE_HEIGHT = HORIZON_HEIGHT//2                       # Height of just the blue/orange centre sections
MIDDLE = HEIGHT//2                                    # Y position of centre of screen

#Segment constant
ARC_HEIGHT = 10
BOTTOM_RECT_HEIGHT = BLACK_HEIGHT - OUTER_GAP - ARC_HEIGHT                             # Height of black rectangle at bottom of screen

#Bottom rectangle calculations
BOTTOM_TOP = BLACK_HEIGHT + HORIZON_HEIGHT + OUTER_GAP + ARC_HEIGHT



start_ms = pygame.time.get_ticks()

running = True
while running:
    for ev in pygame.event.get():
        if ev.type == pygame.QUIT:
            running = False

    screen.fill(BLACK)

    #Blue horizon    
    pygame.draw.rect(screen, BLUE,
                     (HORIZON_LEFT, BLACK_HEIGHT, HORIZON_WIDTH, BLUE_HEIGHT))
    #Orange horizon
    pygame.draw.rect(screen, BROWN,
                     (HORIZON_LEFT, MIDDLE, HORIZON_WIDTH, BLUE_HEIGHT))
    

    # Writing APP and STD labels 
    app_text = app_font.render("APP", False, APP_GREEN)  # False = crisper/blockier
    app_x = HORIZON_LEFT + 8
    app_y = (BLACK_HEIGHT - app_text.get_height()) // 2
    screen.blit(app_text, (app_x, app_y))

    std_text = std_font.render("STD", False, STD_COLOUR)
    std_x = HORIZON_LEFT + HORIZON_WIDTH - std_text.get_width() - 8
    std_y = (BLACK_HEIGHT - std_text.get_height()) // 2
    screen.blit(std_text, (std_x, std_y))




    draw_roll_marks(
    screen,
    HORIZON_LEFT,
    BLACK_HEIGHT,
    HORIZON_WIDTH,
    HORIZON_HEIGHT,
)



    

    # **attitude marks**
    draw_attitude_marks(screen,
                        HORIZON_LEFT, BLACK_HEIGHT,
                        HORIZON_WIDTH, HORIZON_HEIGHT)
    
    # pitch‑bug (static for now; offset 0)
    draw_pitch_bug(screen,
                   HORIZON_LEFT, BLACK_HEIGHT,
                   HORIZON_WIDTH, HORIZON_HEIGHT,
                   pitch_offset=0.0)    
    
    #Drawing the three triangles
    draw_triangles(screen, HORIZON_LEFT, HORIZON_WIDTH)


    
    #Speedtape
    pygame.draw.rect(screen, GREY, 
                     (OUTER_GAP, OUTER_GAP, TAPE_WIDTH, HEIGHT - 2 * OUTER_GAP ))
    #Altitude tape
    pygame.draw.rect(screen, GREY, 
                     ((OUTER_GAP * 2) + TAPE_WIDTH + HORIZON_WIDTH + OUTER_GAP, OUTER_GAP, TAPE_WIDTH, HEIGHT - 2*OUTER_GAP))



  

    # Bottom section: large circle with center far below
    BOTTOM_CIRCLE_RADIUS = HORIZON_WIDTH + 300 # Adjust for roundness (larger = flatter)
    BOTTOM_CIRCLE_CX = HORIZON_LEFT + HORIZON_WIDTH // 2
    BOTTOM_CIRCLE_CY = 895  # Far below screen; increase for flatter arch
    pygame.draw.circle(screen, GREY, (BOTTOM_CIRCLE_CX, BOTTOM_CIRCLE_CY), BOTTOM_CIRCLE_RADIUS)
    #Lateral black rectangle
    pygame.draw.rect(screen, BLACK, 
                 (0, HEIGHT-OUTER_GAP, WIDTH, OUTER_GAP))
    #LHS vertical black line
    pygame.draw.rect(screen, BLACK, 
                 (OUTER_GAP + TAPE_WIDTH, 0, OUTER_GAP, HEIGHT))
    #RHS vertical black line
    pygame.draw.rect(screen, BLACK, 
                 (HORIZON_LEFT + HORIZON_WIDTH, 0, OUTER_GAP, HEIGHT))

    draw_heading_marks(
    screen,
    BOTTOM_CIRCLE_CX,
    BOTTOM_CIRCLE_CY,
    BOTTOM_CIRCLE_RADIUS,
)
    

    # draw_tape_box(
    # screen,
    # tape_left=left_x,
    # tape_top=OUTER_GAP,
    # tape_width=TAPE_WIDTH,
    # tape_height=tape_h,
    # box_width = 100,
    # box_height = 50,
    # )

    # draw_tape_box(
    # screen,
    # tape_left=right_x,
    # tape_top=OUTER_GAP,
    # tape_width=TAPE_WIDTH,
    # tape_height=tape_h,
    # box_width=100,
    # box_height=50,
    # ) 


    tape_h = HEIGHT - 2 * OUTER_GAP
    left_x = OUTER_GAP 
    right_x = WIDTH - 100 -OUTER_GAP


    elapsed_s = (pygame.time.get_ticks() - start_ms) / 1000.0
    ramp = min(elapsed_s / 10.0, 1.0)   # 0.0 to 1.0 over 10 seconds

    current_speed = 40 * ramp
    current_altitude = 400 * ramp

    draw_speed_tape_values(
        screen,
        tape_left=left_x,
        tape_top=OUTER_GAP,
        tape_width=TAPE_WIDTH,
        tape_height=tape_h,
        current_speed=current_speed,   # replace with live speed variable later
        box_width=100,
        box_height=50,
    )

    draw_altitude_tape_values(
        screen,
        tape_left=right_x,
        tape_top=OUTER_GAP,
        tape_width=TAPE_WIDTH,
        tape_height=tape_h,
        current_altitude=current_altitude,  # replace with live altitude variable later
        box_width=100,
        box_height=50,
    )
    
    pygame.display.flip()
    clock.tick(30)

pygame.quit()
