#include <iostream>
#include <chrono>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <unordered_set>

const int C_LIMIT = 50000;

/*
 * Use the Inclusion-Exclusion principle.
 * To avoid a quadractic seach for intersections over all 50,000 cuboids,
 * we slice through the z-axis, considering rectangles in the x-y plane only.
 *
 * The finding of intersections is still really dumb and can likely be improved.
 *
 * Runtime ~400 seconds (6-7 minutes).
 * Quite a way to go for the 1 minute mark.
 */

struct line_seg {
    int start;
    int end;
};

struct cuboid {
    line_seg sx;
    line_seg sy;
    line_seg sz;
};

std::vector<int> create_lfg(int limit) {
    std::vector<int> lfg(limit + 1);
    for (long long int k = 0; k < 56; k++) {
        lfg[k] = (100003ll - 200003ll * k + 300007ll * k * k * k) % 1000000;
    }
    for (long long int k = 56; k < limit + 1; k++) {
        lfg[k] = (lfg[k-24] + lfg[k-55]) % 1000000;
    }
    return lfg;
}

std::vector<cuboid> create_cuboids(const std::vector<int>& lfg) {
    std::vector<cuboid> cuboids(C_LIMIT);
    for (int n = 1; n < C_LIMIT + 1; n++) {
        int sx = lfg.at(6*n - 5) % 10000;
        int sy = lfg[6*n - 4] % 10000;
        int sz = lfg[6*n - 3] % 10000;
        int dx = 1 + lfg[6*n - 2] % 399;
        int dy = 1 + lfg[6*n - 1] % 399;
        int dz = 1 + lfg.at(6*n - 0) % 399;
        line_seg lx = {sx, sx + dx};
        line_seg ly = {sy, sy + dy};
        line_seg lz = {sz, sz + dz};
        cuboids[n-1] = {lx, ly, lz};
    }
    return cuboids;
}

std::vector<cuboid> eliminate_swallowed_xy_rects(const std::vector<cuboid>& cuboids) {
    std::vector<cuboid> rects;
    rects.reserve(cuboids.size());

    for (auto c1 = cuboids.begin(); c1 != cuboids.end(); ++c1) {
        for (auto c2 = cuboids.begin(); c2 != cuboids.end(); ++c2) {
            if (c1 == c2) continue;
            if (c1->sx.start >= c2->sx.start && c1->sx.end <= c2->sx.end
                    && c1->sy.start >= c2->sy.start && c1->sy.end <= c2->sy.end) {
                goto FULLY_CONTAINED;
            }
        }
        rects.push_back(*c1);
    FULLY_CONTAINED:;
    }
    return rects;
}


std::tuple<std::vector<cuboid>, std::vector<std::unordered_set<int>>>
find_xy_intersections(std::vector<cuboid>& cuboids,
        std::vector<cuboid>& intersections,
        std::vector<std::unordered_set<int>> components) {
    // a really dumb search that'll generate duplicate intersections that have to be pruned afterwards
    // e.g. suppose rectangle A is being intersected with B^C, giving A^B^C
    // well B will also be intersected with A^C, to give B^A^C - we have to filter out one of these
    int idx = 0;
    std::vector<cuboid> new_intersections;
    std::vector<std::unordered_set<int>> new_components;
    for (auto r1 : cuboids) {
        auto it = intersections.begin();
        auto cit = components.begin();
        for (; it != intersections.end() && cit != components.end(); ++it, ++cit) {
            if (cit->count(idx)) continue;
            auto r2 = *it;
            int x_start = std::max(r1.sx.start, r2.sx.start);
            int x_end = std::min(r1.sx.end, r2.sx.end);
            if (x_end < x_start) continue;
            int y_start = std::max(r1.sy.start, r2.sy.start);
            int y_end = std::min(r1.sy.end, r2.sy.end);
            if (y_end < y_start) continue;
            line_seg sx = {x_start, x_end}, sy = {y_start, y_end}, sz = {0, 1};
            new_intersections.push_back({sx, sy, sz});
            auto c = *cit;
            c.insert(idx);
            new_components.push_back(c);
        }
        idx++;
    }
    std::vector<cuboid> deduped_intersections;
    std::vector<std::unordered_set<int>> deduped_components;
    for (std::size_t i = 0; i < new_components.size(); i++) {
        for (std::size_t j = i + 1; j < new_components.size(); j++) {
            if (new_components[i] == new_components[j]) goto NEXT_ITEM;
        }
        deduped_components.push_back(new_components[i]);
        deduped_intersections.push_back(new_intersections[i]);
NEXT_ITEM:;
    }
    return std::make_tuple(deduped_intersections, deduped_components);
}

long long int combined_area(std::vector<cuboid> cuboids) {
    long long int sum = 0;
    for (int z_level = 0; z_level < 10400; z_level++) {
        std::vector<cuboid> rects(cuboids.size());
        auto end_it = std::copy_if(cuboids.begin(), cuboids.end(), rects.begin(),
                [z_level](cuboid& c) {return c.sz.start <= z_level && z_level < c.sz.end;});
        rects.resize(std::distance(rects.begin(), end_it));
        rects = eliminate_swallowed_xy_rects(rects);
        auto intersections = rects;
        std::vector<std::unordered_set<int>> components;
        components.reserve(rects.size());
        for (int i = 0; i < rects.size(); i++) components.emplace_back(std::unordered_set<int> {i});

        int include_exclude_factor = 1;
        while (intersections.size() != 0) {
            long long int area = 0;
            for (auto r : intersections) {
                area += abs(r.sx.end - r.sx.start) * abs(r.sy.end - r.sy.start);
            }
            sum += area * include_exclude_factor;
            include_exclude_factor *= -1;
            std::tie(intersections, components) = find_xy_intersections(rects, intersections, components);
        }
        if (z_level % 500 == 0) std::cout << "At level: " << z_level << std::endl;
    }
    return sum;
}

int main() {
    auto startTime = std::chrono::steady_clock::now();

    auto lfg = create_lfg(C_LIMIT * 6);
    std::vector<cuboid> cuboids = create_cuboids(lfg);

    long long int area = combined_area(cuboids);
    std::cout << area << std::endl;

    auto endTime = std::chrono::steady_clock::now();
    auto runTime = endTime - startTime;
    std::cout << "Time: " << std::chrono::duration<double, std::milli> (runTime).count() << " ms" << std::endl;

    return 0;
}
