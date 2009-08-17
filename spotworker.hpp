/*
 * (C) Copyright 2009 Pål Driveklepp
 *
 * Written by: Pål Driveklepp <jalla2000@gmail.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SPOTWORKER_H
#define SPOTWORKER_H

#include <QObject>
#include <QTimer>
#include <QtNetwork>
#include <stdio.h>

#include "alsaworker.hpp"
#include <spotify/api.h>

class QTcpServer;

#define MAX_PACKET_SIZE 10000

class SpotWorker : public QObject {

    Q_OBJECT

 public:
    
    sp_session *currentSession;

    AlsaWorker *alsaWorker;
    SoundSaver *soundSaver;
    QTimer *eventTimer;
    /// A global reference to the search we are currently investigating
    sp_search *g_search;
    enum PacketType { PLAYSTOP = 1, NEXTBLOCK = 2, SKIP = 3, SEEK = 4, SEARCH = 5 };

    static SpotWorker* getInstance();
    ~SpotWorker()
    {
        instanceFlag = false;
    }
    int start(QString username, QString password);

    QString getUsername(void);
    QString getPassword(void);
    void saveUser(QString user, QString pass);

    /*
    void connection_error(sp_session *session, sp_error error);
    void logged_in(sp_session *session, sp_error error);
    void logged_out(sp_session *session);
    void notify_main_thread(sp_session *session);
    void log_message(sp_session *session, const char *data);
    void loop(sp_session *session);
    void sigIgn(int signo);
    */

    void print_track(sp_track *track);
    void print_album(sp_album *album);
    void print_artist(sp_artist *artist);
    void print_search(sp_search *search);

    /* signal-emitting C++ functions
     * for the 9 sp_session_callbacks pointers
     */
    void emitLoggedInSignal(sp_session *session, sp_error error);
    void emitLoggedOutSignal(sp_session *session);
    //TODO: metadata signal
    void emitConnectionErrorSignal(sp_session *session, sp_error error);
    //TODO: message to user signal
    //TODO: notify main thread? (probably no need)
    int emitMusicDeliverySignal(sp_session *session, const sp_audioformat *format, const void *frames, int num_frames);
    void emitPlayTokenLostSignal(sp_session *session);
    //TODO: log_message signal
    
    void emitSessionTerminatedSignal(void);
    void emitSessionReadySignal(sp_session *session);
    void emitSearchCompleteSignal(sp_search *search);

    void performSearch(QString query);
    //These three funnctions had sp_session *session parameter. removal experimental
    void loadPlayer(sp_track *track, bool rip, SoundSaver::FileType type);
    void playPlayer(bool play);
    void seekPlayer(int offset);
    void resetCounter();

    void saveFile(sp_track *track, SoundSaver::FileType nextFile);
    void closeFile(void);
    int getProgress();
    int getSongLength();
    bool isPlaying();
    bool isStreaming();

 private:
    static bool instanceFlag;
    static SpotWorker *workerInstance;
    SpotWorker(QObject *parent = 0);
    int totalFrames;
    int frameCounter;
    QTimer *watchDog;
    bool streaming;
    
    QTcpServer *tcpServer;
    QTcpSocket *clientConnection;
    typedef struct spacket {
	uint32_t state;
	uint32_t type;
	uint32_t length;
	uint32_t xfered;
	uint8_t data[MAX_PACKET_SIZE];
    } spotPacket;
    spotPacket serverData;
    QMutex *controlMutex;
    void parsePacket();

 public slots:
    void processEvents();
    void streamingStopped();
    void playbackStopped();
    void startServer();
    void netConnection();
    void rxDataReady();

 signals:
    void loggedIn(sp_session *session, sp_error error);
    void loggedOut(sp_session *session);
    //TODO: metadata_updated signal
    void connectionError(sp_session *session, sp_error error);
    //TODO: message to user signal
    //TODO: notify main thread? (probably no need)
    void playListsDiscovered(sp_playlistcontainer *plc);
    void playTokenLost(sp_session *session);
    //TODO: log_message signal

    void sessionReady(bool ready);
    void searchComplete(sp_search *search);
    void sessionReady(sp_session *session);
    void sessionTerminated(void);
};

extern "C" int music_delivery(sp_session *session, const sp_audioformat *format, const void *frames, int num_frames);
extern "C" void play_token_lost(sp_session *session);
extern "C" void search_complete(sp_search *search, void *userdata);

//playlist callbacks
extern "C" void playlist_added(sp_playlistcontainer *pc, sp_playlist *pl, int position, void *userdata);
extern "C" void playlist_removed(sp_playlistcontainer *pc, sp_playlist *pl, int position, void *userdata);
extern "C" void tracks_added(sp_playlist *pl, const sp_track **tracks,int num_tracks, int position, void *userdata);
extern "C" void tracks_removed(sp_playlist *pl, const int *tracks, int num_tracks, void *userdata);
extern "C" void tracks_moved(sp_playlist *pl, const int *tracks, int num_tracks, int new_position, void *userdata);
extern "C" void playlist_renamed(sp_playlist *pl, void *userdata);


#endif