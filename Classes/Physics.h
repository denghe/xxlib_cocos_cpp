struct Physics : PKG::CatchFish::Configs::Physics {
	cpSpace* space = nullptr;
	inline int InitCascade() noexcept override {
		assert(!space);
		static_assert(sizeof(cpVect) == sizeof(xx::Pos));
		if (polygons) {
			space = cpSpaceNew();
			auto&& body = cpSpaceGetStaticBody(space);
			for (auto&& polygon : *polygons) {
				auto&& shape = cpPolyShapeNew(body, (int)polygon->len, (cpVect*)polygon->buf, cpTransformIdentity, 0.0);
				cpSpaceAddShape(space, shape);
			}
		}
		return 0;
	}
	~Physics() {
		if (space) {
			cpSpaceEachShape(space, [](cpShape *shape, void *data) {
				cpSpaceAddPostStepCallback((cpSpace*)data, [](cpSpace *space, void *key, void *data) {
					cpSpaceRemoveShape(space, (cpShape *)key);
					cpShapeFree((cpShape *)key);
				}, shape, nullptr);
			}, space);
			cpSpaceFree(space);
			space = nullptr;
		}
	}
};
using Physics_s = std::shared_ptr<Physics>;
