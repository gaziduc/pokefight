#include "pictures.h"
#include "window.h"
#include "texture.h"
#include "window.h"
#include <map>
#include <memory>

Pictures::Pictures() {

}

void Pictures::add_texture(const Window& window, const enum Picture pic_num, const std::string& filename) {
	_pics.emplace(pic_num, std::make_shared<Texture>(window, filename));
}

std::shared_ptr<Texture> Pictures::get_texture(const enum Picture pic_num) const {
	return _pics.at(pic_num);
}