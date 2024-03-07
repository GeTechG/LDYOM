import React, {CSSProperties, useEffect} from 'react';
import Link from "@docusaurus/Link";

const useScript = url => {
    useEffect(() => {
        const script = document.createElement('script');

        script.src = url;
        script.async = true;

        document.body.appendChild(script);

        return () => {
            document.body.removeChild(script);
        }
    }, [url]);
};

export default function () {
    useScript("https://viewer.diagrams.net/js/viewer-static.min.js")
    return (
        <div>
            <img src={require("./logo_Beta071.jpg").default} alt="photo"
                 style={{width: '50vw', margin: 'auto', padding: '20px'}}/>
            <div className="post">
                <p>It turned out that this version, to put it mildly, was delayed from the plan. I've already mentioned
                    the reasons, but I'll repeat. I've been dealing with problems that I couldn't solve for a long time
                    because they pointed to moonloader, and its code is closed. I wrote to the forum many times, either
                    they didn't respond to me, or they told me to look for the problem in my own code. And in the end, I
                    decided to switch to C++, rewriting LDYOM on it, thus I have almost full control over the code,
                    except for GTA itself. Yes, I haven't eliminated all the unknown game crashes. But at least
                    something.</p>
                <p>You might ask, now that LDYOM is in C++ (ASI), what will happen to the name, as the first letter
                    denoted LUA. Nothing, it will remain the same, because I haven't abandoned LUA, the node system in
                    Beta 0.7.1 is written in LUA.</p>
                <div style={{margin: 'auto', textAlign: 'center'}}>
                    <div className="mxgraph" style={{border: "1px solid transparent", margin: "auto"}}
                         data-mxgraph="{&quot;highlight&quot;:&quot;#0000ff&quot;,&quot;lightbox&quot;:false,&quot;nav&quot;:true,&quot;resize&quot;:true,&quot;toolbar&quot;:&quot;zoom&quot;,&quot;edit&quot;:&quot;_blank&quot;,&quot;xml&quot;:&quot;&lt;mxfile host=\&quot;app.diagrams.net\&quot; modified=\&quot;2021-05-01T07:48:55.105Z\&quot; agent=\&quot;5.0 (Windows)\&quot; etag=\&quot;j7NK0kCG96kDsg6IbIiv\&quot; version=\&quot;14.6.8\&quot; type=\&quot;browser\&quot;&gt;&lt;diagram id=\&quot;VP_GMa-ecQAymTiG9F4w\&quot; name=\&quot;Page-1\&quot;&gt;7Vldc+I2FP01PG7HtvzFIwWSbps02SGdbR6FdbHVGMsjRID++sq2jC3kEpKYkO30Ce6RZFnn3nN8MQM0Xm6vOc6TW0YgHTgW2Q7QZOA4NkKO/CiQXYV4rlcBMadETWqAGf0bFGgpdE0JrLSJgrFU0FwHI5ZlEAkNw5yzjT5twVJ91xzHYACzCKcm+p0SkVRo6AQN/gvQOKl3tv1hNbLE9WR1klWCCdu0IDQdoDFnTFTfltsxpAV5NS/Vuqt/Gd3fGIdMnLLg1z8m34Nvt1cPX74xOnr67W5D119QdZVnnK7VgdXNil3NABBJiAoZFwmLWYbTaYP+zNk6I1BsY8momXPDWC5BW4J/gRA7lV28FkxCiVimatQ8ijrdiq15BEfuvy4JzGMQR+apYxVnaW2giLoGtgTBd3IChxQL+qwnH6saivfzGprlF8X0K1i3DdavH0YSmI0M9nVuNwkVMMtxyclGKk7ncUHTdMxSxsu1aIFJgCOJrwRnT9Aambu+Z7nHmH8GLmB7lCs1imqpKoWHKtw0crEDhSUtqdTz+mfXu0QNSwL57s9i/U9eHT6qy5XBZKtFOxW9vfadE2vf/Vy1H/6fHY0P/1OlxzGsaTT7KoEbhgnw99lTH2aDPM1sHKvDbZwOt/HP5jbBJeqZ4FVSrq+Km4yKVqfYLofsIaFZhV7R4izvLGT3xyxk9+XO5lXFazxDQ2eOfN986tpoGg5RT+Xu6M9W1FXuXke57zXQO6/DH8m+Zc+dF1dMafb0LhF4J4og/FQa8AwN3KxxsSgTwHMOosPRa8pyziJYrV4WxhxHT3GZ4bu1kERDdzNKMISLzmbUj0KYL3oSjKs/H/ZxWzBWh2DQufTimzmYPN7dGrzLA4pjjpOxglmNVQXhlMbS8CcRFGmVQEEXlT9hR2pgSQkpddiVSV2bfaQgOEiBbXqW25GBszmWfZHftLClonIsx1PhY2uoMawiqP2q5XPI8V/jdGV0D5wuS1VXoNYlvN38wlM7AKtv9yuXyt4G71oTcib9a9W68n0BtAtwqBWga1sHNVRdsamo/a29vchCQ+aj+6/nM9c+dOof6HRo6tT/UJ2iy+o0aOvUOlWngdaQ2C/ItCdF1kr7+H7kTYp0D14RucEHKLLesyXJ31nxLrnPV2zEg5C4R38e9CFVz+7kry3V0Del6p5NqmZXM76Z3vVK7WIBftTZMJJgOLd66lbsg9JE6OIvFIYmtziV3h+XzrAE+cHyqLOU/zNN5L50j6Ql7OfhJMPmT5jKfJq/stD0Hw==&lt;/diagram&gt;&lt;/mxfile&gt;&quot;}"></div>
                    Diagram of LDYOM operation.
                </div>
                <p>It turned out that this version, to put it mildly, was delayed from the plan. I've already mentioned
                    the reasons, but I'll repeat. I've been dealing with problems that I couldn't solve for a long time
                    because they pointed to moonloader, and its code is closed. I wrote to the forum many times, either
                    they didn't respond to me, or they told me to look for the problem in my own code. And in the end, I
                    decided to switch to C++, rewriting LDYOM on it, thus I have almost full control over the code,
                    except for GTA itself. Yes, I haven't eliminated all the unknown game crashes. But at least
                    something.</p>
                <p>In early April, I released a joke version LDYOM 2077, I planned to demonstrate
                    the upcoming version. In the end, I found out that some people have performance issues. This
                    prompted me to release the version earlier than planned, for more extensive testing. According to
                    the results of the vote, we decided that it would still be <b>0.7.1</b>, not <b>0.7.0.5</b>.</p>
                <div style={{display: "flex", justifyContent: "space-between", margin: "20pt"}}>
                    <img src={require("./result1.jpg").default} alt="Results 1" className="img-card"
                         style={{height: '70px'}}/>
                    <img src={require("./result2.jpg").default} alt="Results 2" className="img-card"
                         style={{height: '70px'}}/>
                </div>
                <p>Finally, let's talk about the novelties, although I didn't plan them. Firstly, of course, the
                    transition to C++, the node system created in LUA, the beginning of an API for modifications (at the
                    moment it doesn't work fully, and is oriented towards the node system). The full list of changes is
                    below.</p>
                <p>Lastly, I want to say that this is the last time I've fundamentally redesigned it, because with C++,
                    there's nowhere else to go.</p>

                <h3>Changes</h3>
                <ul>
                    <li>Transition to C++</li>
                    <li>New rendering for the node system (<a href="https://github.com/Nelarius/imnodes">imnodes</a>)
                    </li>
                    <li>Added a large number of new nodes</li>
                    <li>Added trains</li>
                    <li>New parameters for vehicles</li>
                    <li>New parameters for actors</li>
                    <li>New Cyber theme</li>
                    <li>Backward compatibility for future versions</li>
                    <li>Fixed and added new bugs</li>
                    <li>Many minor changes</li>
                </ul>
            </div>
        </div>
    );
}
