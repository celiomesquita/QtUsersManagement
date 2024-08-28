CXX = g++
QT = `pkg-config --cflags --libs Qt5Widgets Qt5Core Qt5Sql`

CXXFLAGS = -Wall -Wextra -fPIC -I/usr/include/GL -DDEBUG $(QT)
LDFLAGS = -lsqlite3 -lssl -lcrypto $(QT)

SOURCES = main.cpp MainWindow.cpp Database.cpp AuthUtils.cpp HashUtils.cpp UserDB.cpp LoginDialog.cpp UserManagementDialog.cpp

BUILD_DIR = build
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:.cpp=.o))
MOC_HEADERS = MainWindow.h LoginDialog.h UserManagementDialog.h
MOC_SOURCES = $(addprefix $(BUILD_DIR)/moc_, $(MOC_HEADERS:.h=.cpp))
MOC_OBJECTS = $(MOC_SOURCES:.cpp=.o)
DEPENDS = $(OBJECTS:.o=.d)

TARGET = UsersManagement

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to generate ui_MainWindow.h from MainWindow.ui
$(BUILD_DIR)/ui_MainWindow.h: MainWindow.ui
	uic MainWindow.ui -o $(BUILD_DIR)/ui_MainWindow.h

$(BUILD_DIR)/ui_UserManagementDialog.h: UserManagementDialog.ui
	uic UserManagementDialog.ui -o $(BUILD_DIR)/ui_UserManagementDialog.h

$(BUILD_DIR)/ui_LoginDialog.h: LoginDialog.ui
	uic LoginDialog.ui -o $(BUILD_DIR)/ui_LoginDialog.h

# Ensure .cpp files depend on the generated .h files
$(BUILD_DIR)/MainWindow.o: MainWindow.cpp $(BUILD_DIR)/ui_MainWindow.h
$(BUILD_DIR)/LoginDialog.o: LoginDialog.cpp $(BUILD_DIR)/ui_LoginDialog.h
$(BUILD_DIR)/UserManagementDialog.o: UserManagementDialog.cpp $(BUILD_DIR)/ui_UserManagementDialog.h

# MOC files generation
$(BUILD_DIR)/moc_%.cpp: %.h
	moc $< -o $@

# MOC objects compilation
$(BUILD_DIR)/moc_%.o: $(BUILD_DIR)/moc_%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS) $(MOC_OBJECTS)
	$(CXX) $(OBJECTS) $(MOC_OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.d: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -M $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(DEPENDS)

clean:
	rm -rf $(TARGET) $(BUILD_DIR)
