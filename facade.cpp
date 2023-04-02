#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

struct Pair
{
    int a;
    int b;
    Pair() = default;
};

class Video
{
    std::string name;
    int length;
    std::string extention;
    
public:
    Video(const std::string& n, int l, const std::string& e) : name(n), length(l), extention(e)
    {}
    void sayName() const {std::cout << name;}
    Pair convertLength() const {return Pair{length / 60, length % 60};}
    void prepareScreen() const;
    std::string getExtention() const {return extention;}
    int getLength() const {return length;}
};

void Video::prepareScreen() const
{
    if(extention == ".mp4")
    {
        std::cout << "!____________________________________________!\n";
    }
    else if (extention == ".mkv")
    {
        std::cout << "!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!_!\n"; 
    }
    else
    {
        std::cout << "Ok, let's start...\n";
    }
}

class Audio
{
    std::vector<int> notes;
    int length;
    int muteStart, muteFinish, noiseStart, noiseFinish;
public:
    Audio() = default;
    Audio(int muteStart, int muteFinish, int noiseStart, int noiseFinish, int length) :
        muteStart(muteStart), muteFinish(muteFinish), noiseStart(noiseStart), noiseFinish(noiseFinish), length(length)
    {
        std::vector<int> t(length, 1);
        notes = t;
    }
    void generateNoise(int start, int finish)
    {
        if(start >= 0 && start < length && start < finish)
        {
            int end = std::min(length, finish);
            for(int i = start; i < end; ++i)
            {
                notes.at(i) = std::rand() % 100;
            }
        }
    }
    void mute(int start, int finish) 
    {
        if(start >= 0 && start < length && start <= finish)
        {
            int end = std::min(length, finish);
            for(int i = start; i < end; ++i)
            {
                notes.at(i) = 0;
            }
        }
    }
    void convert() 
    {
        std::transform(notes.begin(), notes.end(), notes.begin(), [](int i) -> int { return i * 2 + 1; });
    }

    Pair getNoiseParameters() const {return Pair{noiseStart, noiseFinish};}
    Pair getMuteParameters() const {return Pair{muteStart, muteFinish};}
    int getLength() const {return length;}
    std::vector<int>& getNotes() {return notes;}
};

class VideoPlayback
{
    Video& video;
    Audio& audio;
public:
    VideoPlayback(Video& v, Audio& a) : video(v), audio(a) {}
    void play();
};

void VideoPlayback::play()
{
    // moved all this logic from main() 

    video.prepareScreen();
    video.sayName();
    std::cout << video.getExtention() << std::endl;
    Pair p = video.convertLength();
    std::cout << "This video lasts " << p.a << " minutes " << p.b << " seconds\n";
    if(video.getExtention() == ".mp4")
    {
        audio.convert();
    }
    audio.generateNoise(audio.getNoiseParameters().a, audio.getNoiseParameters().b);
    audio.mute(audio.getMuteParameters().a, audio.getMuteParameters().b);
    std::vector<int> nt = audio.getNotes();
    for(int i = 0; i < std::min(audio.getLength(), video.getLength()); ++i)
    {
        std::cout << nt.at(i) << " ";
    }
    std::cout << std::endl;
    video.prepareScreen();

}

int main()
{
    Video v1{"Funny cats", 100, ".mp4"};
    Audio a1{1, 8, 15, 35, 98};

    Video v2{"Disapproving corgies", 200, ".mkv"};
    Audio a2{5, 15, 99, 123, 162};

    // made a facade: one string for object creation and one for function call
    VideoPlayback vp1(v1, a1);
    vp1.play();

    VideoPlayback vp2(v2, a2);
    vp2.play();
}