from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class gamepackagesRecipe(ConanFile):
    name = "gamepackages"
    version = "1.0"
    package_type = "library"

    # Optional metadata
    author = "Lightbow Interactive TM https://lightbowgames.com"
    url = "https://github.com/pascalgluth/GamePackagesLib"
    topics = ("gamedev", "packages", "assets")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "Source/*", "Include/*"

    def requirements(self):
        self.requires("zlib/1.3")
        self.requires("cryptopp/8.8.0")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["gamepackages"]

    

    

