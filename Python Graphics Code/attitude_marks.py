import pygame

def draw_attitude_marks(
    surf: pygame.Surface,
    horizon_left: int,
    horizon_top: int,
    horizon_width: int,
    horizon_height: int,
    colour: tuple[int,int,int] = (255, 255, 255)
) -> None:
    """Draw the attitude scale on `surf` inside the given rectangle.

    The rectangle is defined by its top‑left corner and size.
    """
    centre_y = horizon_top + horizon_height / 2
    px_per_deg = horizon_height / 35.0          # 30‑degree span

    # small font used for the numeric labels; pygame.font is ready
    font = pygame.font.SysFont(None, 18)

    for idx in range(-6, 7):                   # –6…+6 → –15…+15 in 2.5° steps
        deg = idx * 2.5

        y = centre_y - deg * px_per_deg

        # choose length based on degree value:
        #   0° = full width
        #  ±10° = longest
        #  ±5° and ±15° = medium
        #  other (±2.5°, ±12.5°) = shortest
        if deg == 0.0:
            length = horizon_width
        elif abs(deg) == 10:
            length = horizon_width * 0.30      # longest
        elif abs(deg) in (5, 15):
            length = horizon_width * 0.20      # medium
        else:
            length = horizon_width * 0.10      # shortest

        x1 = horizon_left + (horizon_width - length) / 2
        x2 = x1 + length

        pygame.draw.line(
            surf, colour,
            (int(x1), int(y)), (int(x2), int(y)),
            2  # line thickness
        )
        
        
        # draw the 10° label to the left of the long tick
        if abs(deg) == 10:
            label = font.render("10", True, colour)
            lx = int(x1) - label.get_width() - 4
            ly = int(y - label.get_height() / 2)
            surf.blit(label, (lx, ly))    