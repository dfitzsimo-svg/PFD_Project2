import math
import pygame

WHITE = (255, 255, 255)


def _point_on_arc(cx: int, cy: int, radius: int, angle_deg: float) -> tuple[int, int]:
    # 0 deg = straight up, negative = left, positive = right
    a = math.radians(angle_deg)
    x = cx + radius * math.sin(a)
    y = cy - radius * math.cos(a)
    return int(x), int(y)


def draw_roll_marks(
    surf: pygame.Surface,
    horizon_left: int,
    horizon_top: int,
    horizon_width: int,
    horizon_height: int,
    colour: tuple[int, int, int] = WHITE,
) -> None:
    """
    Top roll scale:
    - apex touches the top of the blue horizon
    - arc spans full horizon width
    """
    cx = horizon_left + horizon_width / 2
    apex_y = horizon_top

    # How far lower the arc is at left/right edges than at center.
    # Increase for more curve, decrease for flatter.
    edge_drop = 18.0

    half_span = horizon_width / 2.0

    # Circle from chord/segment geometry
    radius = (half_span * half_span + edge_drop * edge_drop) / (2.0 * edge_drop)
    cy = apex_y + radius + 10

    # Angle where arc hits left/right horizon edges
    end_deg = math.degrees(math.asin(half_span / radius))

    start_i = int(-end_deg)
    end_i = int(end_deg)

    arc_points = [_point_on_arc(int(cx), int(cy), int(radius), d) for d in range(start_i, end_i + 1)]
    #pygame.draw.lines(surf, colour, False, arc_points, 2)

    # Static roll tick marks across the arc
    tick_fracs = [-1.0, -0.75, -0.5, -0.33, -0.16, 0.0, 0.16, 0.33, 0.5, 0.75, 1.0]
    for f in tick_fracs:
        ang = f * end_deg
        tick_len = 14 if abs(f) in (1.0, 0.5, 0.0) else 9
        p_outer = _point_on_arc(int(cx), int(cy), int(radius), ang)
        p_inner = _point_on_arc(int(cx), int(cy), int(radius - tick_len), ang)
        pygame.draw.line(surf, colour, p_outer, p_inner, 2)


def draw_heading_marks(
    surf: pygame.Surface,
    circle_cx: int,
    circle_cy: int,
    circle_radius: int,
    colour: tuple[int, int, int] = WHITE,
) -> None:
    # Bottom curved heading marks on the grey arch
    arc_points = [_point_on_arc(circle_cx, circle_cy, circle_radius, d) for d in range(-14, 15, 1)]
    pygame.draw.lines(surf, colour, False, arc_points, 2)

    for ang in range(-12, 13, 3):
        tick_len = 12 if ang % 6 == 0 else 7
        p_outer = _point_on_arc(circle_cx, circle_cy, circle_radius, ang)
        p_inner = _point_on_arc(circle_cx, circle_cy, circle_radius - tick_len, ang)
        pygame.draw.line(surf, colour, p_outer, p_inner, 2)