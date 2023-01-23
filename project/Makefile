CXX := g++

PROJECT := bridget

EXEDIR := bin
SRCDIR := src
OBJDIR := obj

CXXSRC := $(notdir $(wildcard $(SRCDIR)/*.cpp))
SOURCES :=  $(CXXSRC)
OBJECTS := $(CXXSRC:.cpp=.o)

INCLUDE := header

CXXFLAGS := -O2 -Wall -I./$(INCLUDE)
MMFLAGS := -I./$(INCLUDE) -MM

RM := rm -rf

# $@ <- ターゲット名
# $^ <- 依存ファイルのリスト
# $< <- 依存ファイルの1つ目

# 実行ファイルへの統合
$(EXEDIR)/$(PROJECT) : $(addprefix $(OBJDIR)/,$(OBJECTS))
		@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
		$(CXX) -o $@ $^


# オブジェクトファイルへのコンパイル
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp 
		@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
		$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY : all clean run debug
all : clean $(EXEDIR)/$(PROJECT)

clean :
		$(RM) $(OBJDIR) $(EXEDIR)

run : 
		./$(EXEDIR)/$(PROJECT)

debug :
		$(warning SOURCES = $(SOURCES))
		$(warning OBJECTS = $(OBJECTS))

# 依存関係の出力
dpd : $(addprefix $(SRCDIR)/,$(SOURCES))
		$(CXX) $(MMFLAGS) $^


