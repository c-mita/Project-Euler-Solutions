#include <iostream>
#include <chrono>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <unordered_set>
#include <set>

const int C_LIMIT = 50000;

/*
 * Three candidate solutions:
 * Uncomment the desired define below to select
 */


/* Use the Inclusion-Exclusion principle on all cuboids at once.
 * Find all pair-wise intersections, iteratively intersect intersections
 * to produce next n-tuple (discarding duplicates or (n+1)-tuples, etc.
 *
 * Performs very well for C_LIMIT < 20,000
 * C_LIMIT = 50,000 -> Runtime >100 seconds
 */
//#define THREED_CUBOID_INTERSECTIONS


/*
 * Slice along the z-axis, considering only the x-y plane.
 * Perform the same intersection strategy as the 3D case, iteratively intersecting
 * whilst recording the "component" rectangles to avoid duplications and mis-counting.
 * Use the Inclusion-Exclusion principle to build up the area for each slice, summing for
 * each z_level to calculate the volume.
 *
 * Performs slightly better than 3D case for large C_LIMIT (>30,000), worse otherwise
 * C_LIMIT = 50,000 -> Runtiime ~90 seconds
 */
//#define TWOD_RECTANGLE_INTERSECTIONS

/*
 * Slice along the z-axis, considering only the x-y plane.
 * Order all rectangle corner positions, tracking original rectangle
 * and whether its an "open" or "closing" point.
 * Sweep a line along the x-axis, stopping at each point to calculate the
 * intersection of "current" rectangles with the line (multiplying by the distance
 * to the next corner)
 *
 * Performs much better than the other cases for large C_LIMIT (>20,000), but
 * is slower than the 3D case for smaller C_LIMIT.
 * C_LIMIT = 50,000 -> Runtime < 15 seconds
 * An acceptable solution
 */
#define X_SWEEP_ON_XY_PLANES


struct line_seg {
    int start;
    int end;
};

struct cuboid {
    line_seg sx;
    line_seg sy;
    line_seg sz;
};


#ifdef TWOD_RECTANGLE_INTERSECTIONS

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

#endif

#ifdef THREED_CUBOID_INTERSECTIONS

std::tuple<std::vector<cuboid>, std::vector<std::unordered_set<int>>>
find_intersections(std::vector<cuboid>& boxes, std::vector<std::unordered_set<int>> components) {
    std::vector<cuboid> intersections;
    std::vector<std::unordered_set<int>> new_components;
    // dumb quadratic intersection algorithm, discarding intersections with the "wrong number" of intersections
    if (components.size() == 0) return std::make_tuple(intersections, new_components);
    std::size_t n_components = components[0].size() + 1;
    auto c = components.begin();
    int i = 0;
    for (auto b1 = boxes.begin(); b1 != boxes.end() && c != components.end(); ++b1, ++c, ++i) {
        int j = i + 1;
        for (auto b2 = b1 + 1; b2 != boxes.end(); ++b2, ++j) {
            int x_start = std::max(b1->sx.start, b2->sx.start);
            int x_end = std::min(b1->sx.end, b2->sx.end);
            if (x_end < x_start) continue;
            int y_start = std::max(b1->sy.start, b2->sy.start);
            int y_end = std::min(b1->sy.end, b2->sy.end);
            if (y_end < y_start) continue;
            int z_start = std::max(b1->sz.start, b2->sz.start);
            int z_end = std::min(b1->sz.end, b2->sz.end);
            if (z_end < z_start) continue;
            line_seg sx = {x_start, x_end}, sy = {y_start, y_end}, sz = {z_start, z_end};
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
    auto intersections = cuboids;
    std::vector<std::unordered_set<int>> components;
    components.reserve(intersections.size());
    for (int i = 0; i < intersections.size(); i++) components.emplace_back(std::unordered_set<int> {i});

    int include_exclude_factor = 1;
    while (intersections.size() != 0) {
        long long int v = 0;
        for (auto b : intersections) {
            v += abs(b.sx.end - b.sx.start) * abs(b.sy.end - b.sy.start) * abs(b.sz.end - b.sz.start);
        }
        sum += v * include_exclude_factor;
        include_exclude_factor *= -1;
        std::tie(intersections, components) = find_intersections(intersections, components);
    }

    return sum;
}

#endif


#ifdef X_SWEEP_ON_XY_PLANES

long long int combined_area(std::vector<cuboid> cuboids) {
    static const bool OPEN = false;
    static const bool CLOSE = true;
    long long int sum = 0;
    // separate out x-axis segments of cuboids into start and stop and order according to this single value
    // we will iterate through a subset of these at each z-level and want to stop at each distinct x-value"
    std::vector<std::tuple<int, bool, cuboid*>> arranged_cuboids(cuboids.size() * 2);
    auto end_it = std::transform(cuboids.begin(), cuboids.end(), arranged_cuboids.begin(),
            [](cuboid& c) -> std::tuple<int, bool, cuboid*> {return std::make_tuple(c.sx.start, OPEN, &c);});
    end_it = std::transform(cuboids.begin(), cuboids.end(), end_it,
            [](cuboid& c) -> std::tuple<int, bool, cuboid*> {return std::make_tuple(c.sx.end, CLOSE, &c);});
    std::sort(arranged_cuboids.begin(), arranged_cuboids.end(),
            [](std::tuple<int, bool, cuboid*> c1, std::tuple<int, bool, cuboid*> c2) -> bool {return std::get<0>(c1) < std::get<0>(c2);});

    for (int z_level = 0; z_level < 10400; z_level++) {
        // filter out everything not intersecting with current x-y plane
        std::vector<std::tuple<int, bool, cuboid*>> filtered(arranged_cuboids.size());
        end_it = std::copy_if(arranged_cuboids.begin(), arranged_cuboids.end(), filtered.begin(),
                [z_level](std::tuple<int, bool, cuboid*> p) {return std::get<2>(p)->sz.start <= z_level && z_level < std::get<2>(p)->sz.end;});
        filtered.resize(std::distance(filtered.begin(), end_it));

        // sweep a line along x-axis
        int area = 0;
        std::set<std::tuple<int, cuboid*, bool>> seen_points;
        for (auto it1 = filtered.begin(), it2 = filtered.begin() + 1;
                it1 != filtered.end() && it2 != filtered.end();
                ++it1, ++it2) {
            // first we add new "opening" points and remove "closing" points from our set
            std::tuple<int, bool, cuboid*>& point_x = *it1;
            if (std::get<1>(point_x) == OPEN) {
                seen_points.insert(std::make_tuple(std::get<2>(point_x)->sy.start, std::get<2>(point_x), OPEN));
                seen_points.insert(std::make_tuple(std::get<2>(point_x)->sy.end, std::get<2>(point_x), CLOSE));
            } else if (std::get<1>(point_x) == CLOSE) {
                seen_points.erase(std::make_tuple(std::get<2>(point_x)->sy.start, std::get<2>(point_x), OPEN));
                seen_points.erase(std::make_tuple(std::get<2>(point_x)->sy.end, std::get<2>(point_x), CLOSE));
            }

            // run through current segments in order to calculate intersection with sweeping line
            int opened = 0;
            int length_y = 0;
            int start_y = 0;
            for (std::tuple<int, cuboid*, bool> point : seen_points) {
                if (opened == 0 && std::get<2>(point) == OPEN) start_y = std::get<0>(point);
                if (std::get<2>(point) == CLOSE) {
                    opened--;
                } else if (std::get<2>(point) == OPEN) {
                    opened++;
                }
                if (opened == 0) {
                    length_y += std::get<0>(point) - start_y;
                }
            }
            area += length_y * (std::get<0>(*it2) - std::get<0>(point_x));
        }
        sum += area;
        if (z_level % 500 == 0) std::cout << "At level: " << z_level << ", Current volume: " << sum << std::endl;
    }
    return sum;
}

#endif


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
