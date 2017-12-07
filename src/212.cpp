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
 * We intersect all pairs of rectangles, then iteratively intersect those to
 * get all 3-tuple intersections, and so on.
 * We keep track of what rectangles "make up" an intersection, discarding those
 * intersections that have the "wrong number" of parents (so A^B ^ C^D isn't
 * counted as a 3-tuple). We also have to discard duplicate intersections.
 *
 * Runtime ~90 seconds (1.5 minutes).
 * Needs Improvement to get to the one minute mark.
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

std::tuple<std::vector<cuboid>, std::vector<std::unordered_set<int>>>
find_xy_intersections(std::vector<cuboid>& rects, std::vector<std::unordered_set<int>> components) {
    std::vector<cuboid> intersections;
    std::vector<std::unordered_set<int>> new_components;
    // dumb quadratic intersection algorithm, discarding intersections with the "wrong number" of intersections
    if (components.size() == 0) return std::make_tuple(intersections, new_components);
    std::size_t n_components = components[0].size() + 1;
    auto c = components.begin();
    int i = 0;
    for (auto r1 = rects.begin(); r1 != rects.end() && c != components.end(); ++r1, ++c, ++i) {
        int j = i + 1;
        for (auto r2 = r1 + 1; r2 != rects.end(); ++r2, ++j) {
            int x_start = std::max(r1->sx.start, r2->sx.start);
            int x_end = std::min(r1->sx.end, r2->sx.end);
            if (x_end < x_start) continue;
            int y_start = std::max(r1->sy.start, r2->sy.start);
            int y_end = std::min(r1->sy.end, r2->sy.end);
            if (y_end < y_start) continue;
            line_seg sx = {x_start, x_end}, sy = {y_start, y_end}, sz = {0, 1};
            std::unordered_set<int> c_new = components[i];
            c_new.insert(components[j].begin(), components[j].end());
            if (c_new.size() == n_components) {
                intersections.push_back({sx, sy, sz});
                new_components.push_back(c_new);
            }
        }
    }

    // filter out duplicate intersections
    std::vector<cuboid> deduped_intersections;
    std::vector<std::unordered_set<int>> deduped_components;
    for (int i = 0; i < new_components.size(); i++) {
        for (int j = i + 1; j < new_components.size(); j++) {
            if (new_components[i] == new_components[j]) goto NEXT_ITEM;
        }
        deduped_components.push_back(new_components[i]);
        deduped_intersections.push_back(intersections[i]);
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
            std::tie(intersections, components) = find_xy_intersections(intersections, components);
        }
        if (z_level % 500 == 0) std::cout << "At level: " << z_level << ", Current volume: " << sum << std::endl;
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
