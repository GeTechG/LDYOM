import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import Layout from '@theme/Layout';
import HomepageFeatures from '@site/src/components/HomepageFeatures';

import styles from './index.module.css';
import useBaseUrl from "@docusaurus/useBaseUrl";
import data from "@site/data";

function HomepageHeader() {
    const {siteConfig} = useDocusaurusContext();
    return (
        <div>
            <div className={styles.my_header}>
                <video className={styles.slider} autoPlay={true} loop={true} preload="auto">
                    <source src={useBaseUrl('/media/back.webm')} type='video/webm; codecs="vp8, vorbis"'/>
                    <source src={useBaseUrl('/media/back.mp4')} type='video/mp4'/>
                </video>
                <div style={{margin: 'auto', display: 'grid'}}>
                    <h1 style={{color: 'white', fontWeight: 100, margin: 0, textAlign: 'center'}}>LDYOM</h1>
                    <h3 style={{color: 'white', fontWeight: 300, textAlign: 'center', margin: 0}}>Beta {data.latest_version}</h3>
                    <br/>
                    {
                        data["pre_version"] ?
                        (<h5 style={{color: 'white', fontWeight: 200, textAlign: 'center'}}>{data["pre_version"]}</h5>)
                        : null
                    }
                    <a href="download" className={styles.button}>Download</a>
                </div>
            </div>
        </div>
    );
}

export default function Home(): JSX.Element {
    const {siteConfig} = useDocusaurusContext();
    return (
        <Layout
            description="Create your world">
            <HomepageHeader/>
            <main>
                <HomepageFeatures/>
            </main>
        </Layout>
    );
}
