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
animation filler::FillBFS(FillerConfig& config) {
  // complete your implementation below
  // You should replace the following line with a
  // correct call to fill.
 
  return Fill<Queue>(config); // REPLACE THIS STUB
}

/*
*  Performs a flood fill using depth first search.
*
*  PARAM:  config - FillerConfig struct to setup the fill
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
animation filler::FillDFS(FillerConfig& config) {
  // complete your implementation below
  // You should replace the following line with a
  // correct call to fill.
  
  return Fill<Stack>(config); // REPLACE THIS STUB
}

/*
*  Run a flood fill on an image starting at the seed point
*
*  PARAM:  config - FillerConfig struct with data for flood fill of image
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
template <template <class T> class OrderingStructure> animation filler::Fill(FillerConfig& config)
{
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

  int framecount = 0; // increment after processing one pixel; used for producing animation frames (step 3 above)
  animation anim;
  OrderingStructure<PixelPoint> os;

  PNG img = config.img;
  PixelPoint seedpoint = config.seedpoint;
  double tolerance = config.tolerance;
  ColorPicker* picker = config.picker;
  PriorityNeighbours neighbourorder = config.neighbourorder;
  neighbourorder.SetReferenceColor(seedpoint.color);

  // THOUGHT PROCESS: 
  // 1. insert n e s w into vector (via .Insert(n e s w))
  // 2. while vector is not empty --> remove each point by calling .Remove on priorityneighbours
  //    --> this will return one pixelpoint that satisfies condition best each time, until no more Pixelpoints are left (aka 4 times)
  // 3. place each of these pixels onto our os 
  // 4. set processed[][] = 1
  // 5. each iteration, pass in this pixelpoint to our colorpicker
  // 6. replace original pixel with the modified pixel

  // *** we need guards for:
  //        1. checking if pixel position is within image size
  //        2. checking if it meets tolerance req
  //        3. checking if it has already been processed (remember to set processed[][] = 1)
  //        4. checking if pixel position is >= 0

  int w = img.width();
  int h = img.height();
  int processedPoints[w][h];
  
  // set all element arrays to 0
  for (int i = 0; i < w; i++) { 
    for (int j = 0; j < h; j++) {
      processedPoints[i][j] = 0;
    }
  }

  // process seedpoint (add to os)
  os.Add(seedpoint);
  processedPoints[seedpoint.x][seedpoint.y] = 1;
  // change that pixel on the image
  if (seedpoint.x >= 0 && seedpoint.x < w && seedpoint.y >= 0 && seedpoint.y < h) {

    HSLAPixel* first = img.getPixel(seedpoint.x, seedpoint.y);
    *first = picker->operator()(PixelPoint(seedpoint.x, seedpoint.y,*first));
    framecount++;

    // pass in a copy of the modified image
    if (framecount % config.frameFreq == 0) {
      anim.addFrame(PNG(img));
    }
  }

  while(!os.IsEmpty()) {
    // Remove the Pixelpoint on top of the stack and put its neighbours (n e s w) in vector if:
    //    1. the neighbouring point position is within image size
    //    2. the neighbouring point position is not negative
    //    3. the neighbouring point has not been processed yet
    PixelPoint curr = os.Remove();
    // neighbourorder.SetReferenceColor(curr.color);

    if (curr.x < w && curr.y-1 < h && curr.x >=0 && curr.y-1 >= 0) {
      HSLAPixel north = *img.getPixel(curr.x, curr.y-1);
      if (processedPoints[curr.x][curr.y-1] != 1) {
        neighbourorder.Insert(PixelPoint(curr.x, curr.y-1, north));
      }
    }

    if (curr.x < w && curr.y+1 < h && curr.x >=0 && curr.y+1 >= 0) {
      HSLAPixel south = *img.getPixel(curr.x, curr.y+1);
      if (processedPoints[curr.x][curr.y+1] != 1) {
        neighbourorder.Insert(PixelPoint(curr.x, curr.y+1, south));
      }
    }

    if (curr.x+1 < w && curr.y < h && curr.x+1 >=0 && curr.y >= 0) {
      HSLAPixel east = *img.getPixel(curr.x+1, curr.y);
      if (processedPoints[curr.x+1][curr.y] != 1) {
        neighbourorder.Insert(PixelPoint(curr.x+1, curr.y, east));
      }
    }

    if (curr.x-1 < w && curr.y < h && curr.x-1 >=0 && curr.y >= 0) {
      HSLAPixel west = *img.getPixel(curr.x-1, curr.y);
      if (processedPoints[curr.x-1][curr.y] != 1) {
        neighbourorder.Insert(PixelPoint(curr.x-1, curr.y, west));
      }
    }
    // ^^^ finished adding neighbours to the vector

    neighbourorder.SetReferenceColor(curr.color);
    
    // process all the vectors and add them to the queue in priority order, IF its within tolerance
    while (!neighbourorder.IsEmpty()) {

      PixelPoint next = neighbourorder.Remove(); // this returns pixelpoint that we are gonna work with
      // neighbourorder.SetReferenceColor(next.color);

      if ((next.color).dist(seedpoint.color) <= tolerance) { // checking for tolerance requirement 
        HSLAPixel* currPixelOnImg = img.getPixel(next.x, next.y); // pixel on the image
        *currPixelOnImg = picker->operator()(next); // mofified pixel, via a colorpicker of user's choice

        os.Add(next);
        processedPoints[next.x][next.y] = 1;
        framecount++;
      }

      // add a copy of the modified image if (framecount % config.frameFreq == 0)
      if (framecount % config.frameFreq == 0) {
        anim.addFrame(PNG(img));
      }
    }
  }

  // add copy of the final frame and return animation
  anim.addFrame(PNG(img));
  return anim;
}