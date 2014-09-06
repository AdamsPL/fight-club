#include <Config.h>

#include <gtest/gtest.h>

const QString defaultFilename = "example.cfg";
const QString missingFilename = "dummyFile.cfg";

TEST(TestConfig, getValueWhenNoFileLoaded)
{
	Config conf;

	EXPECT_TRUE(conf.getValue("randomValue").isEmpty());
}

TEST(TestConfig, getValueWhenNotExistingFile)
{
	Config conf;

	conf.loadFromFile(missingFilename);

	EXPECT_TRUE(conf.getValue("foo").isEmpty());
}

TEST(TestConfig, getValueAfterExistingFileWasLoaded)
{
	Config conf;

	conf.loadFromFile(defaultFilename);

	EXPECT_TRUE(conf.getValue("randomValue").isEmpty());

	EXPECT_EQ("none", conf.getValue("first"));
	EXPECT_EQ("none", conf.getValue("second"));
	EXPECT_EQ("foo", conf.getValue("foo"));
}
