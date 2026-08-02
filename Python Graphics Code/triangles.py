# triangle.py
import pygame

def draw_triangles(surface: pygame.Surface, horizon_left: int, horizon_width: int) -> None:
    """Draw the three hard‑coded triangles originally in car_pfd.py."""
    BLACK = (0, 0, 0)
    WHITE = (255, 255, 255)

    # triangle 1 (pointing up)
    tri_base = 20
    tri_height = 15
    cx = horizon_left + horizon_width // 2
    cy = 60
    p1 = (cx - tri_base//2, cy + tri_height)
    p2 = (cx + tri_base//2, cy + tri_height)
    p3 = (cx, cy)
    pygame.draw.polygon(surface, BLACK, (p1, p2, p3))
    pygame.draw.polygon(surface, WHITE, (p1, p2, p3), width=3)

    # triangle 2 (pointing down, white fill)
    tri_base = 20
    tri_height = 15
    cx = horizon_left + horizon_width // 2
    cy = 40
    p1 = (cx - tri_base//2, cy)
    p2 = (cx + tri_base//2, cy)
    p3 = (cx, cy + tri_height)
    pygame.draw.polygon(surface, WHITE, (p1, p2, p3))
    pygame.draw.polygon(surface, WHITE, (p1, p2, p3), width=3)

    # triangle 3 (pointing down, black fill)
    tri_base = 20
    tri_height = 15
    cx = horizon_left + horizon_width // 2
    cy = 285
    p1 = (cx - tri_base//2, cy)
    p2 = (cx + tri_base//2, cy)
    p3 = (cx, cy + tri_height)
    pygame.draw.polygon(surface, BLACK, (p1, p2, p3))
    pygame.draw.polygon(surface, WHITE, (p1, p2, p3), width=3)