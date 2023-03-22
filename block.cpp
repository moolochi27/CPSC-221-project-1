#include "block.h"
#include <cmath>
#include <iostream>

int Block::height() const {
  /* your code here */
  return data[0].size();;
}

int Block::width() const {
  /* your code here */
  return data.size();
}

void Block::render(PNG &im, int x) const {
  /* your code here */
  for(int i = 0; i < (int) data.size(); i++){
    for(int j = 0; j < (int) im.height(); j++){
        *im.getPixel(x+i,j) = data[i][j];
    }
  }
}

void Block::build(PNG &im, int x, int width) {
  /* your code here */
  for(int i = x; i < x + width; i++){
    vector<HSLAPixel> temp;
    for(int j = 0; j< (int) im.height(); j++){
      temp.push_back(*im.getPixel(i,j));
    }
    data.push_back(temp);
  }
}
