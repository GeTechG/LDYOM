import React from 'react';
import Link from "@docusaurus/Link";

export default function () {
    return (
        <div>
            <img src={require("./logo_Beta072.jpg").default} alt="photo"
                 style={{width: '50vw', margin: 'auto', padding: '20px'}}/>
            <div className="post">
                <p>The version turned out almost as planned, but as always, it was delayed. There is still a lot that I
                    would like to add. But for now, the main goal is to reach a stable version and gradually move
                    towards the release. I don't know how many more versions will be needed to achieve this, but I think
                    at least 2 more, and maybe more.</p>
                <p>The version brought many new nodes, not all that I wanted, but still. Even something that was not
                    planned was added, namely <b>visual effects</b>.</p>
                <p>The next version will also aim to expand the list of nodes. I don't know which ones yet, as it won't
                    be possible to add everything, as the functionality is very extensive.</p>
                <p>Some may wonder, what is the new <b>LCAPI</b> (LDYOM-CLEO API)? - This should be something like
                    another legacy after me. It is planned to interact with LDYOM through CLEO scripting. Most likely, I
                    won't spend much time on it, as it will take a lot of the already limited time. But that doesn't
                    mean you can't work with it. Already now, there is access to mission memory, and if desired, you can
                    make some additions for LDYOM. This is already the second tool along with the nodes, for additional
                    modding, as I will never cover the entire spectrum of your desires.</p>
                <p>Remember that we have a <Link to="https://discord.gg/4KGf4Px">Discord server</Link>. And I suggest
                    you write there whether it is worth writing such posts at all. Also, you can communicate on
                    LDYOM-related topics there, publish your missions, etc.</p>

                <h3>Changes</h3>
                <ul>
                    <li>New nodes for pedestrians, vehicles, etc.</li>
                    <li>Visual effects</li>
                    <li>New objectives</li>
                    <li>Backward compatibility for future versions</li>
                    <li>LCAPI addon</li>
                    <li>Fixed and added new <span style={{textDecoration: 'line-through'}}>bugs</span> features</li>
                    <li>Many minor changes</li>
                </ul>
            </div>
        </div>
    );
}
