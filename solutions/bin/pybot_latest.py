import sys
from collections import deque

TURN_NUMBER = 0
EGG_COEF = 40

def bfs(cells, source_index, destination_index):
    visited = set()
    queue = deque()
    queue.append((cells[source_index], 0, [source_index]))  # Include the source index in the initial path

    while queue:
        current_cell, distance, path = queue.popleft()

        if current_cell.index == destination_index:
            return distance, path
        
        visited.add(current_cell)
        for neighbor_index in current_cell.neighbors:
            neighbor_cell = cells[neighbor_index]
            if neighbor_cell not in visited:
                queue.append((neighbor_cell, distance + 1, path + [neighbor_index]))  # Append neighbor index to the path
    return -1, []  # Return -1 and an empty path if no path is found

def sort_cells_by_distance(cells):
    sorted_cells = sorted(cells, key=lambda cell: cell.dist_to_my_base)
    return sorted_cells

def display_cells(cells):
    for cell in cells:
        cell_type = "eggs" if cell.cell_type == 1 else "crys"
        path = "; ".join([str(idx) for idx in cell.path_to_my_base])
        print(f"Type: {cell_type} Idx: {cell.index}, Dist: {cell.dist_to_my_base}, Score:{round(cell.score, 2)}, Path:[{path}]", file=sys.stderr)

def sort_potential_targets(cells, targets):
    egg_coef = max(1 - (min(TURN_NUMBER, EGG_COEF) / EGG_COEF), 0.1)
    crys_coef = min(1 - egg_coef, 0.9)
    scores = []
    for target in targets:
        eggs_rate = 0
        crys_rate = 0
        ants_to_carry = 0
        ants_rate = 0
        ants_malus = 0
        score = 0.0

        for id_in_path in target.path_to_my_base:
            cell_in_path = cells[id_in_path]
            ants_to_carry += cell_in_path.my_ants
        ants_rate = ants_to_carry / len(target.path_to_my_base)

        for id_in_path in target.path_to_my_base:
            cell_in_path = cells[id_in_path]
            if cell_in_path.cell_type == 1:
                eggs_rate += min(ants_rate, cell_in_path.resources)
            elif cell_in_path.cell_type == 2:
                crys_rate += min(ants_rate, cell_in_path.resources)
            # if cell_in_path.my_ants == 0:
        ants_malus = target.dist_to_my_base / 6

        score = eggs_rate * egg_coef + crys_rate * crys_coef
        if score > 0:
            score += ((ants_rate / 5) - ants_malus)
        target.score = max(score, 0)
        scores.append(f"{round(score, 1)}\t base: {target.my_closest_base} idx{target.index}: d={target.dist_to_my_base} s={round(target.score, 3)}, eggr={round(eggs_rate, 1)} cryr={round(crys_rate, 1)} antr={round(ants_rate, 1)} mal={ants_malus}")

    scores.append("________________")
    for score_msg in sorted(scores, reverse=True):
        print(score_msg, file=sys.stderr)

    by_path_res = sorted(cells, key=lambda cell: cell.score, reverse=True)
    return by_path_res

def decide_beacons_on_path(cells, target):
    actions = []
    len(target.path_to_my_base)
    path = []
    for id_in_path in target.path_to_my_base:
        cell = cells[id_in_path]
        path.append(cell)
        actions.append(f'BEACON {cell.index} 1')
    
    # sorted(path, key=lambda cell: cell.my_ants, reverse=True)
    # previous_ants_amount = -1
    # strenght = 1
    # for cell in path:
    #     print(f"{target.index}: {cell.index}({cell.my_ants}, str compute{strenght})", file=sys.stderr)
    #     if cell.my_ants < previous_ants_amount:
    #         strenght += 1
    #     actions.append(f'BEACON {cell.index} {strenght}')
    #     previous_ants_amount = cell.my_ants
    return actions

class Cell(object):
    index: int
    cell_type: int
    resources: int
    neighbors: list[int]
    my_ants: int
    opp_ants: int
    my_closest_base: int
    opp_closest_base: int
    dist_to_my_base: int
    dist_to_opp_base: int
    path_to_my_base: list[int]
    path_to_opp_base: list[int]
    score: float

    def __init__(self, index: int, cell_type: int, resources: int, neighbors: list[int], my_ants: int, opp_ants: int):
        self.index = index
        self.cell_type = cell_type
        self.resources = resources
        self.neighbors = neighbors
        self.my_ants = my_ants
        self.opp_ants = opp_ants
        self.my_closest_base = -1
        self.opp_closest_base = -1
        self.dist_to_my_base = -1
        self.dist_to_opp_base = -1
        self.path_to_my_base = []
        self.path_to_opp_base = []
        self.score = 0.0

cells: list[Cell] = []

number_of_cells = int(input())  # amount of hexagonal cells in this map
for i in range(number_of_cells):
    inputs = [int(j) for j in input().split()]
    cell_type = inputs[0] # 0 for empty, 1 for eggs, 2 for crystal
    initial_resources = inputs[1] # the initial amount of eggs/crystals on this cell
    neigh_0 = inputs[2] # the index of the neighbouring cell for each direction
    neigh_1 = inputs[3]
    neigh_2 = inputs[4]
    neigh_3 = inputs[5]
    neigh_4 = inputs[6]
    neigh_5 = inputs[7]
    cell: Cell = Cell(
        index = i,
        cell_type = cell_type,
        resources = initial_resources,
        neighbors = list(filter(lambda id: id > -1,[neigh_0, neigh_1, neigh_2, neigh_3, neigh_4, neigh_5])),
        my_ants = 0,
        opp_ants = 0
    )
    cells.append(cell)
number_of_bases = int(input())
my_bases: list[int] = []
for i in input().split():
    my_base_index = int(i)
    my_bases.append(my_base_index)
opp_bases: list[int] = []
for i in input().split():
    opp_base_index = int(i)
    opp_bases.append(opp_base_index)

# compute closest bases
for base in my_bases:
    for cell in cells:
        dist, path = bfs(cells, base, cell.index)
        if (cell.dist_to_my_base == -1 or dist < cell.dist_to_my_base):
            cell.dist_to_my_base, cell.path_to_my_base = dist, path
            cell.my_closest_base = base
for base in opp_bases:
    for cell in cells:
        dist, path = bfs(cells, base, cell.index)
        if (cell.dist_to_opp_base == -1 or dist < cell.dist_to_opp_base):
            cell.dist_to_opp_base, cell.path_to_opp_base = dist, path
            cell.opp_closest_base = base

cells.sort(key=lambda cell: cell.index)
# game loop
while True:
    scores = [int(j) for j in input().split()]
    my_total_ants = 0
    opp_total_ants = 0
    for i in range(number_of_cells):
        inputs = [int(j) for j in input().split()]
        resources = inputs[0] # the current amount of eggs/crystals on this cell
        my_ants = inputs[1] # the amount of your ants on this cell
        opp_ants = inputs[2] # the amount of opponent ants on this cell

        cells[i].resources = resources
        cells[i].my_ants = my_ants
        my_total_ants += my_ants
        cells[i].opp_ants = opp_ants
        opp_total_ants += opp_ants
        cells[i].score = 0.0

    # WAIT | LINE <sourceIdx> <targetIdx> <strength> | BEACON <cellIdx> <strength> | MESSAGE <text>
    actions = []

    saved_targets = []
    for base in my_bases:
        max_target = int(min(len([t for t in cells if t.resources > 0]) / 2 + 1, my_total_ants / 10))
        order_given = 0
        potential_targets = [cell for cell in sort_cells_by_distance(cells) if cell.resources > 0 and cell.my_closest_base == base and cell not in saved_targets]
        potential_targets = sort_potential_targets(cells, potential_targets)
        # print(f"max_t: {max_target} for base {base} : len considered = {len(potential_targets)}", file=sys.stderr, flush=True)
        # for pot in potential_targets:
        #     print(f"    idx: {pot.index},\tscore: {pot.score}", file=sys.stderr, flush=True)
        for cell in potential_targets:
            if order_given < max_target and cell.score > 0:
                saved_targets.append(cell)
                order_given += 1
                actions += decide_beacons_on_path(cells, cell)
                # print(f"idx{cell.index}: d={cell.dist_to_my_base} s={cell.score}", file=sys.stderr)
            elif len(actions) >= max_target:
                break

    if len(actions) == 0:
        print('WAIT')
    else:
        print(';'.join(actions))
    TURN_NUMBER += 1
