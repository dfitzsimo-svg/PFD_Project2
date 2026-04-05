# pitch_bug.py
import pygame

def draw_pitch_bug(
    surf: pygame.Surface,
    horizon_left: int,
    horizon_top: int,
    horizon_width: int,
    horizon_height: int,
    pitch_offset: float = 0.0,
    bar_length: int = 40,          # horizontal bar length
    bar_thick: int = 8,           # bar thickness
    leg_length: int = 10,          # vertical leg length
    leg_thick: int = 9,            # vertical leg thickness (new param)
    square_size: int = 10,
    gap: int = 20,                 # gap between square and bars
    bg_colour: tuple[int,int,int] = (0, 0, 0),
    outline_colour: tuple[int,int,int] = (255, 255, 255),
    outline_thick: int = 2,        # ✅ thickness of white outline
) -> None:
    """
    Draw a pitch‑bug composed of two sideways L’s and a centre square.

    The whole assembly is centred in the horizon area and shifted vertically by
    `pitch_offset` (pixels).  All dimensions may be tuned via parameters.
    """
    cy = horizon_top + horizon_height / 2 + pitch_offset
    cx = horizon_left + horizon_width / 2

    # central square
    half_sq = square_size / 2
    sq_rect = pygame.Rect(cx - half_sq, cy - half_sq,
                          square_size, square_size)

    # left L – horizontal bar extends left; vertical leg on the **right**
    # end of that bar (i.e. facing center).
    left_h_end = cx - half_sq - gap
    left_h_start = left_h_end - bar_length
    left_pts = [
        (left_h_start, cy - bar_thick / 2),
        (left_h_end, cy - bar_thick / 2),
        (left_h_end, cy + bar_thick / 2 + leg_length),
        (left_h_end - leg_thick, cy + bar_thick / 2 + leg_length),
        (left_h_end - leg_thick, cy + bar_thick / 2),
        (left_h_start, cy + bar_thick / 2),
    ]

    # right L – horizontal bar extends right; vertical leg on the **left**
    # end of that bar (mirror of the left L).
    right_h_start = cx + half_sq + gap
    right_h_end = right_h_start + bar_length
    right_pts = [
        (right_h_start, cy - bar_thick / 2),
        (right_h_end, cy - bar_thick / 2),
        (right_h_end, cy + bar_thick / 2),
        (right_h_start + leg_thick, cy + bar_thick / 2),
        (right_h_start + leg_thick, cy + bar_thick / 2 + leg_length),
        (right_h_start, cy + bar_thick / 2 + leg_length),
    ]

    # draw filled shapes
    pygame.draw.polygon(surf, bg_colour, left_pts)
    pygame.draw.polygon(surf, bg_colour, right_pts)
    pygame.draw.rect(surf, bg_colour, sq_rect)

    # draw outlines using the configured thickness
    pygame.draw.lines(surf, outline_colour, True, left_pts, outline_thick)
    pygame.draw.lines(surf, outline_colour, True, right_pts, outline_thick)
    pygame.draw.rect(surf, outline_colour, sq_rect, outline_thick)