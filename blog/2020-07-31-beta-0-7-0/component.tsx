import React from 'react';
import Link from "@docusaurus/Link";

export default function () {
    return (
        <div>
            <img src={require("./logo_Beta070.jpg").default} alt="photo"
                 style={{width: '50vw', margin: 'auto', padding: '20px'}}/>
            <div className="post">
                <p>Finally, the long-awaited LDYOM Beta 0.7.0 has been released; One of the reasons for this is the
                    previously released teaser. Honestly, I didn't think it would cause such a stir, as it was just a
                    sign that I'm continuing to work, that's all.</p>
                <div style={{margin: 'auto', marginBottom: '40px', marginTop: '40px', textAlign: "center"}}>
                    <iframe width="560" height="315" src="https://www.youtube.com/embed/SRHdHA8yQ5Q" frameBorder="0"
                            allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture"
                            allowFullScreen></iframe>
                </div>
                <p>This update turned out to be quite significant, as it brought two major innovations at once, namely a
                    storyline and a node editor. Also, the return to the previously used GUI - Imgui. You can download
                    it on the <Link to="/download">Downloads</Link> page.</p>
                <p>I cannot fail to mention the appearance of this website along with the <Link
                    to="https://mgeymer.github.io/LDYOM_DOC/" target="_blank">manual</Link> and <Link
                    to="https://ldyom.boards.net/" target="_blank">forum</Link>. The forum's creator is not me, but a
                    community user. Therefore, he will be responsible for the forum. In the manual, you can find
                    instructions for installation and creating a simple mission. For now, the manual is available
                    in <b>Russian</b> and <b>English</b>. Anyone willing to translate it into other languages, I can
                    fork the documentation on <Link to="https://github.com/MGeymer/LDYOM_DOC"
                                                    target="_blank">GitHub</Link> and translate it.</p>
                <p>And, as always, I've prepared a video for the release of the new version, but unfortunately, it's in
                    Russian. Those who don't understand it can try the automatic subtitles. But in general, what I
                    talked about in the video is mentioned here. I hope you like the new node editor; I demonstrated its
                    principle of operation in a <Link to="https://mgeymer.github.io/LDYOM_DOC/en/Basics/simple_mission/"
                                                      target="_blank">simple mission</Link>, and I also plan to make a
                    couple more tutorials, but more detailed in explaining its operation.</p>
                <div style={{margin: 'auto', marginBottom: '40px', marginTop: '40px', textAlign: "center"}}>
                    <iframe width="560" height="315" src="https://www.youtube.com/embed/L3K_8zk_ai4" frameBorder="0"
                            allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture"
                            allowFullScreen></iframe>
                </div>
                <p>I'll touch a bit on the next version Beta 0.7.1; It will expand the capabilities of nodes and fix any
                    found bugs. This is preliminary what I plan, as the top priority now is to ensure the stability of
                    LDYOM, then comes the functionality and everything else. So, I urge you to download and test it, as
                    I received many messages: <i>"Where can I download and try it?"</i>, but there were some problems in
                    the old version that hindered some in the installation.</p>
                <h3>Changes</h3>
                <ul>
                    <li>New interface</li>
                    <li>Language support fixed</li>
                    <li>Added node editor</li>
                    <li>Added storyline mode</li>
                    <li>New way to select skins, cars, weapons</li>
                    <li>Bug fixes</li>
                </ul>
            </div>
        </div>
    );
}
