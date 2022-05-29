/*
*  File:        filler.cpp
*  Description: Implementation of functions in the filler namespace.
*
*/

/*
*  Performs a flood fill using breadth first search.
*
*  PARAM:  config - FillerConfig struct to setup the fill
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
animation filler::FillBFS(FillerConfig &config) {
    animation anim;
    anim = Fill<Queue>(config);
    return anim;
}

/*
*  Performs a flood fill using depth first search.
*
*  PARAM:  config - FillerConfig struct to setup the fill
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
animation filler::FillDFS(FillerConfig &config) {
    animation anim;
    anim = Fill<Stack>(config);
    return anim;
}

/*
*  Run a flood fill on an image starting at the seed point
*
*  PARAM:  config - FillerConfig struct with data for flood fill of image
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
template<template<class T> class OrderingStructure>
animation filler::Fill(FillerConfig &config) {


    /*
    * You need to implement this function!
    *
    * This is the basic description of a flood-fill algorithm: Every fill
    * algorithm requires an ordering structure, which is passed to this
    * function via its template parameter. For a breadth-first-search
    * fill, that structure is a Queue, for a depth-first-search, that
    * structure is a Stack. To begin the algorithm, you simply place the
    * given point in the ordering structure, marking it processed
    * (the way you mark it is a design decision you'll make yourself).
    * We have a choice to either change the color, if appropriate, when we
    * add the point to the OS, or when we take it off. In our test cases,
    * we have assumed that you will change the color when a point is added
    * to the structure.
    * Until the structure is empty, you do the following:
    *
    * 1.     Remove a point from the ordering structure, and then...
    *
    *        1.    add its unprocessed neighbors (up/down/left/right) whose color values are
    *              within (or equal to) tolerance distance from the seed point,
    *              to the ordering structure, and
    *              mark them as processed.
    *        2.    if it is an appropriate frame, send the current PNG to the
    *              animation (as described below).
    *
    * 2.     When implementing your breadth-first-search and
    *        depth-first-search fills, you will need to explore neighboring
    *        pixels (up/down/left/right) in some order.
    *
    *        While the order in which you examine neighbors does not matter
    *        for a proper fill, you must use the same order as we do for
    *        your animations to come out like ours! The order you should put
    *        neighboring pixels **ONTO** the queue or stack is based on the
    *        following priority condition:
    *        ** MINIMUM COLOR DISTANCE FROM THE CURRENT PIXEL **
    *        Ties are broken first by minimum y-coordinate, then by minimum x-coordinate.
    *        The HSLAPixel.dist() function will be useful, and you should
    *        take advantage of the functionality in your PriorityNeighbours class.
    *
    *        If you process the neighbours in a different order, your fill may
    *        still work correctly, but your animations will be different
    *        from the grading scripts!
    *
    * 3.     For every k pixels filled, **starting at the kth pixel**, you
    *        must add a frame to the animation, where k = frameFreq.
    *
    *        For example, if frameFreq is 4, then after the 4th pixel has
    *        been filled you should add a frame to the animation, then again
    *        after the 8th pixel, etc.  You must only add frames for the
    *        number of pixels that have been filled, not the number that
    *        have been checked. So if frameFreq is set to 1, a pixel should
    *        be filled every frame.
    *
    * 4.     Finally, as you leave the function, send one last frame to the
    *        animation. This frame will be the final result of the fill, and
    *        it will be the one we test against.
    *
    */

    animation anim;
    OrderingStructure<PixelPoint> os;

    // complete your implementation below
    // HINT: you will likely want to declare some kind of structure to track
    //       which pixels have already been visited

    int processed = 0;
    vector<PixelPoint> visited;


    HSLAPixel pixel = config.picker->operator()(config.seedpoint);
    HSLAPixel *imgPixel = config.img.getPixel(config.seedpoint.x, config.seedpoint.y);
    *imgPixel = pixel;
    os.Add(config.seedpoint);
    processed++;

    PriorityNeighbours priority = config.neighbourorder;

    while (!os.IsEmpty()) {
        PixelPoint curr = os.Remove();
        priority.SetReferenceColor(curr.color);

        if (checkVisited(curr, visited)) {
            AddNeighboursToPriority(curr, priority, config);
            visited.push_back(curr);
        }

        while (!priority.IsEmpty()) {
            PixelPoint neighbour = priority.Remove();
            if(neighbour.color.dist(config.seedpoint.color) <= config.tolerance) {
                HSLAPixel pixel = config.picker->operator()(neighbour);
                HSLAPixel *imgPixel = config.img.getPixel(neighbour.x, neighbour.y);
                *imgPixel = pixel;
                os.Add(neighbour);
                processed++;
            }
            if (processed == config.frameFreq) {
                anim.addFrame(config.img);
                processed = 0;
            }
        }
    }

    anim.addFrame(config.img);
    return anim;
}


bool filler::checkVisited(PixelPoint & curr, const vector<PixelPoint> & visited) {
    for (PixelPoint p : visited) {
        if (p.x == curr.x && p.y == curr.y) {
            return false;
        }
    }
    return true;
}


void filler::AddNeighboursToPriority(PixelPoint &curr, PriorityNeighbours &priority, FillerConfig &config) {
    if (curr.y > 0) {
        HSLAPixel *upColor = config.img.getPixel(curr.x, curr.y - 1);
        PixelPoint upPixel(curr.x, curr.y - 1, *upColor);
        priority.Insert(upPixel);
    }

    if (curr.y < config.img.height() - 1) {
        HSLAPixel *downColor = config.img.getPixel(curr.x, curr.y + 1);
        PixelPoint downPixel(curr.x, curr.y + 1, *downColor);
        priority.Insert(downPixel);
    }

    if (curr.x < config.img.width() - 1) {
        HSLAPixel *rightColor = config.img.getPixel(curr.x + 1, curr.y);
        PixelPoint rightPixel(curr.x + 1, curr.y, *rightColor);
        priority.Insert(rightPixel);
    }

    if (curr.x > 0) {
        HSLAPixel *leftColor = config.img.getPixel(curr.x - 1, curr.y);
        PixelPoint leftPixel(curr.x - 1, curr.y, *leftColor);
        priority.Insert(leftPixel);
    }

}


