class GearDetection:
    hsv_lower: tuple[int, int, int]
    hsv_upper: tuple[int, int, int]
    px_radius: int
    px_area: int

    def __init__(self, hsv_lower: tuple[int, int, int], hsv_upper: tuple[int, int, int], radius: int, area=200):
        self.hsv_lower = hsv_lower
        self.hsv_upper = hsv_upper
        self.px_radius = radius
        self.px_area = area