# tape_boxes.py
import pygame

def draw_tape_box(
    surf: pygame.Surface,
    tape_left: int,
    tape_top: int,
    tape_width: int,
    tape_height: int,
    *,
    box_width: int = 50,
    box_height: int = 30,
    fill_colour: tuple[int, int, int] = (0, 0, 0),
    outline_colour: tuple[int, int, int] = (255, 255, 255),
    outline_thick: int = 3,
) -> None:
    # Keep box inside the tape width.


    # Touch the tape's outer edge.
    x = tape_left
    y = tape_top + (tape_height - box_height) // 2

    rect = pygame.Rect(x, y, box_width, box_height)
    pygame.draw.rect(surf, fill_colour, rect)
    pygame.draw.rect(surf, outline_colour, rect, width=outline_thick)


def draw_speed_tape_values(
    surf: pygame.Surface,
    tape_left: int,
    tape_top: int,
    tape_width: int,
    tape_height: int,
    current_speed: float,
    *,
    box_width: int = 100,
    box_height: int = 50,
    step: int = 10,
    line_spacing: int = 40,
    text_colour: tuple[int, int, int] = (255, 255, 255),
    fill_colour: tuple[int, int, int] = (0, 0, 0),
    outline_colour: tuple[int, int, int] = (255, 255, 255),
    outline_thick: int = 3,
) -> None:
    """Draw speed tape labels with the current speed in a centered black box."""
    display_speed = max(0, int(round(current_speed)))

    box_x = tape_left
    box_y = tape_top + (tape_height - box_height) // 2
    box_rect = pygame.Rect(box_x, box_y, box_width, box_height)

    pygame.draw.rect(surf, fill_colour, box_rect)
    pygame.draw.rect(surf, outline_colour, box_rect, width=outline_thick)

    big_font = pygame.font.SysFont("arialroundedcondensed", 30)
    small_font = pygame.font.SysFont("arialroundedcondensed", 20)

    speed_text = big_font.render(str(display_speed), False, text_colour)
    speed_rect = speed_text.get_rect(midright=box_rect.center)
    surf.blit(speed_text, speed_rect)

    clip_rect = pygame.Rect(tape_left, tape_top, tape_width, tape_height)
    previous_clip = surf.get_clip()
    surf.set_clip(clip_rect)

    tape_bottom = tape_top + tape_height
    centre_y = tape_top + tape_height // 2
    max_rows = tape_height // (2 * line_spacing) + 2

    for i in range(1, max_rows + 1):
        up_val = display_speed + i * step
        down_val = max(0, display_speed - i * step)

        up_y = centre_y - i * line_spacing
        down_y = centre_y + i * line_spacing

        if tape_top <= up_y <= tape_bottom:
            up_text = small_font.render(str(up_val), False, text_colour)
            up_x = tape_left + tape_width - up_text.get_width() - 10
            surf.blit(up_text, (up_x, up_y - up_text.get_height() // 2))

        if tape_top <= down_y <= tape_bottom:
            down_text = small_font.render(str(down_val), False, text_colour)
            down_x = tape_left + tape_width - down_text.get_width() - 10
            surf.blit(down_text, (down_x, down_y - down_text.get_height() // 2))

    surf.set_clip(previous_clip)


def draw_altitude_tape_values(
    surf: pygame.Surface,
    tape_left: int,
    tape_top: int,
    tape_width: int,
    tape_height: int,
    current_altitude: float,
    *,
    box_width: int = 100,
    box_height: int = 50,
    step: int = 100,
    line_spacing: int = 40,
    text_colour: tuple[int, int, int] = (255, 255, 255),
    fill_colour: tuple[int, int, int] = (0, 0, 0),
    outline_colour: tuple[int, int, int] = (255, 255, 255),
    outline_thick: int = 3,
) -> None:
    """Draw altitude tape labels with the current altitude in a centered black box."""
    display_alt = max(0, int(round(current_altitude)))

    box_x = tape_left
    box_y = tape_top + (tape_height - box_height) // 2
    box_rect = pygame.Rect(box_x, box_y, box_width, box_height)

    pygame.draw.rect(surf, fill_colour, box_rect)
    pygame.draw.rect(surf, outline_colour, box_rect, width=outline_thick)

    big_font = pygame.font.SysFont("arialroundedcondensed", 25)
    small_font = pygame.font.SysFont("arialroundedcondensed", 20)

    alt_text = big_font.render(str(display_alt), False, text_colour)
    alt_rect = alt_text.get_rect(center=box_rect.center)
    surf.blit(alt_text, alt_rect)

    clip_rect = pygame.Rect(tape_left, tape_top, tape_width, tape_height)
    previous_clip = surf.get_clip()
    surf.set_clip(clip_rect)

    tape_bottom = tape_top + tape_height
    centre_y = tape_top + tape_height // 2
    max_rows = tape_height // (2 * line_spacing) + 2

    for i in range(1, max_rows + 1):
        up_val = display_alt + i * step
        down_val = max(0, display_alt - i * step)

        up_y = centre_y - i * line_spacing
        down_y = centre_y + i * line_spacing

        if tape_top <= up_y <= tape_bottom:
            up_text = small_font.render(str(up_val), False, text_colour)
            up_x = tape_left + 20
            surf.blit(up_text, (up_x, up_y - up_text.get_height() // 2))

        if tape_top <= down_y <= tape_bottom:
            down_text = small_font.render(str(down_val), False, text_colour)
            down_x = tape_left + 20
            surf.blit(down_text, (down_x, down_y - down_text.get_height() // 2))

    surf.set_clip(previous_clip)    